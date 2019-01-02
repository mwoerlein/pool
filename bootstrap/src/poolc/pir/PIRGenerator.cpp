#include "poolc/pir/PIRGenerator.hpp"

#include "poolc/ast/Type.hpp"
#include "poolc/ast/nodes/all.hpp"

#include "poolc/ast/scopes/UnitScope.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"
#include "poolc/ast/scopes/BlockScope.hpp"
#include "poolc/ast/scopes/VariableScope.hpp"

// public
PIRGenerator::PIRGenerator(Environment &env, MemoryInfo &mi)
        :Object(env, mi), LoggerAware(env, mi),
         curMethod(0), lastValue(0),
         lastLocations(env.create<LinkedList<PIRLocation>>()){}
PIRGenerator::~PIRGenerator() {
    lastLocations.destroy();
}


bool PIRGenerator::visit(TranslationUnitNode & translationUnit) {
    crit() << translationUnit.name << ": PIRGenerator only works for classes, not for translation units!\n";
    return false;
}

bool PIRGenerator::visit(ClassDeclNode & classDecl) {
    if (!classDecl.scope) {
        crit() << classDecl.name << ": classes must be resolved before methods!\n";
        return false;
    }
    
    classDecl.methods.acceptAll(*this);
    return true;
}

bool PIRGenerator::visit(MethodDeclNode & methodDecl) {
    if (methodDecl.kind == abstract) {
        return true;
    }
    MethodScope *scope = methodDecl.scope->isMethod();
    scope->pir = curMethod = &env().create<PIRMethod>();
    curMethod->setLogger(*logger);
    curMethod->init(*scope);
    methodDecl.body.accept(*this);
    curMethod = 0;
    return true;
}

bool PIRGenerator::visit(VariableDeclNode & variableDecl) {
    VariableScope *scope = variableDecl.scope->isVariable();
    if (scope->parent->isBlock()) {
        scope->pir = &curMethod->newTemp(*variableDecl.resolvedType);
    }
    return true;
}

bool PIRGenerator::visit(BlockInstNode & block) {
    block.instructions.acceptAll(*this);
    return true;
}

bool PIRGenerator::visit(ExpressionInstNode & expressionInst) {
    expressionInst.expression.accept(*this);
    lastLocations.clear();
    return true;
}

bool PIRGenerator::visit(InlinePasmInstNode & pasmInstruction) {
    curMethod->addAsm(pasmInstruction.pasm);
    return true;
}

bool PIRGenerator::visit(ReturnInstNode & returnInst) {
    Iterator<ExpressionNode> &it = returnInst.values.iterator();
    int idx = 0;
    while (it.hasNext()) {
        ExpressionNode &ex = it.next();
        ex.accept(*this);
        if (lastLocations.size() == 1) {
            curMethod->addMove(curMethod->asTemp(*lastLocations.first()), *curMethod->getRet(idx++));
            lastLocations.clear();
        } else {
            crit() << "unexpected value " << ex << " in " << returnInst << "\n";
            return false;
        }
    }
    it.destroy();
    curMethod->addReturn();
    return true;
}

bool PIRGenerator::visit(VariableInitInstNode & variableInit) {
    variableInit.variables.acceptAll(*this);
    
    variableInit.initializer.accept(*this);
    if (variableInit.variables.size() == 1) {
        PIRLocation *dest = variableInit.variables.first()->scope->isVariable()->pir;
        if (lastLocations.size() == 1) {
            curMethod->addMove(curMethod->asTemp(*lastLocations.first()), *dest);
            lastLocations.clear();
        } else if (lastValue) {
            curMethod->addAssign(*lastValue, *dest);
            lastValue = 0;
        } else {
            crit() << "unexpected initializer " << variableInit.initializer << " in " << variableInit << "\n";
            return false;
        }
    } else {
        if (lastLocations.size() == variableInit.variables.size()) {
            Iterator<VariableDeclNode> &vit = variableInit.variables.iterator();
            Iterator<PIRLocation> &lit = lastLocations.iterator();
            while (vit.hasNext()) {
                PIRLocation *dest = vit.next().scope->isVariable()->pir;
                curMethod->addMove(curMethod->asTemp(lit.next()), *dest);
            }
            lit.destroy();
            vit.destroy();
            lastLocations.clear();
        } else {
            crit() << "unexpected initializer " << variableInit.initializer << " in " << variableInit << "\n";
            return false;
        }
    }
    // TODO: compare initializer.resolvedType with variables.resolvedType
    return true;
}

bool PIRGenerator::visit(AssignmentExprNode & assignment) {
    if (assignment.variable.context) {
        PIRLocation *ctx = 0;
        assignment.variable.context->accept(*this);
        if (lastLocations.size() == 1) {
            ctx = &curMethod->asTemp(*lastLocations.first());
            lastLocations.clear();
        } else {
            crit() << "unexpected context " << *assignment.variable.context << " " << lastLocations.size() << " in " << assignment << "\n";
            return false;
        }
        assignment.value.accept(*this);
        if (lastLocations.size() == 1) {
            PIRLocation &src = curMethod->asTemp(*lastLocations.first());
            curMethod->addSet(*ctx, *assignment.variable.resolvedVariable, src);
            lastLocations.clear();
            lastLocations.add(src);
        } else {
            crit() << "unexpected value " << assignment.value << " in " << assignment << "\n";
            return false;
        }
    } else {
        PIRLocation *dest = assignment.variable.resolvedVariable->pir;
        assignment.value.accept(*this);
        if (lastLocations.size() == 1) {
            curMethod->addMove(curMethod->asTemp(*lastLocations.first()), *dest);
            lastLocations.clear();
        } else if (lastValue) {
            curMethod->addAssign(*lastValue, *dest);
            lastValue = 0;
        } else {
            crit() << "unexpected value " << assignment.value << " in " << assignment << "\n";
            return false;
        }
        lastLocations.add(*dest);
    }
    return true;
}

bool PIRGenerator::visit(ConstCStringExprNode & constCString) {
    lastValue = &curMethod->getConstString(constCString);
    return true;
}

bool PIRGenerator::visit(ConstIntExprNode & constInt) {
    lastValue = &curMethod->getConstInt(constInt);
    return true;
}

bool PIRGenerator::visit(MethodCallExprNode & methodCall) {
    methodCall.context.accept(*this);
    PIRLocation *context;
    if (lastLocations.size() == 1) {
        context = &curMethod->asTemp(*lastLocations.first());
        lastLocations.clear();
    } else {
        crit() << "unexpected context " << methodCall.context << " " << lastLocations.size() << " for " << methodCall << "\n";
        return false;
    }
    
    LinkedList<PIRLocation> &params = env().create<LinkedList<PIRLocation>>();
    {
        Iterator<ExpressionNode> &it = methodCall.parameters.iterator();
        while (it.hasNext()) {
            ExpressionNode &ex = it.next();
            ex.accept(*this);
            if (lastLocations.size() == 1) {
                params.add(curMethod->asTemp(*lastLocations.first()));
                lastLocations.clear();
            } else if (lastValue) {
                PIRLocation &tmp = curMethod->newTemp(*ex.resolvedType);
                curMethod->addAssign(*lastValue, tmp);
                params.add(tmp);
                lastValue = 0;
            } else {
                crit() << "unexpected parameter " << ex << " in " << methodCall << "\n";
                return false;
            }
        }
        it.destroy();
    }
    LinkedList<PIRLocation> &rets = env().create<LinkedList<PIRLocation>>();
    {
        Iterator<Type> &it = methodCall.resolvedMethod->getMethodDeclNode()->resolvedReturns.iterator();
        while (it.hasNext()) {
            PIRLocation &loc = curMethod->newTemp(it.next());
            lastLocations.add(loc);
            rets.add(loc);
        }
        it.destroy();
    }
    curMethod->addCall(*context, *methodCall.resolvedMethod, params, rets);
    return true;
}

bool PIRGenerator::visit(ThisExprNode & constThis) {
    lastLocations.add(*curMethod->getThis());
    return true;
}

bool PIRGenerator::visit(VariableExprNode & variable) {
    if (variable.context) {
        variable.context->accept(*this);
        if (lastLocations.size() == 1) {
            PIRLocation &tmp = curMethod->newTemp(*variable.resolvedType);
            curMethod->addGet(curMethod->asTemp(*lastLocations.first()), *variable.resolvedVariable, tmp);
            lastLocations.clear();
            lastLocations.add(tmp);
        } else {
            crit() << "unexpected context " << *variable.context << " in " << variable << "\n";
            return false;
        }
    } else {
        lastLocations.add(*variable.resolvedVariable->pir);
    }
    return true;
}
