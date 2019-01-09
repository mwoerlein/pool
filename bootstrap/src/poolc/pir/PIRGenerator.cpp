#include "poolc/pir/PIRGenerator.hpp"

#include "sys/collection/HashMap.hpp"

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
    if (scope->getBlock()) {
        scope->pir = &curMethod->newTemp(*variableDecl.resolvedType);
    }
    // method parameters are handled in PIRMethod::init
    // instance variables and consts have no location
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

bool PIRGenerator::visit(InlinePasmInstNode & pasmInst) {
    HashMap<String, PIRLocation> &in = env().create<HashMap<String, PIRLocation>>();
    {
        Iterator<String> &it = pasmInst.in.keys();
        while (it.hasNext()) {
            String &reg = it.next();
            ExpressionNode &ex = pasmInst.in.get(reg);
            ex.accept(*this);
            if (lastLocations.size() == 1) {
                in.set(reg, curMethod->asTemp(*lastLocations.first()));
                lastLocations.clear();
            } else if (lastValue) {
                PIRLocation &tmp = curMethod->newTemp(*ex.resolvedType);
                curMethod->addAssign(*lastValue, tmp);
                in.set(reg, tmp);
                lastValue = 0;
            } else {
                crit() << "unexpected input expression " << ex << " in " << pasmInst << "\n";
                return false;
            }
        }
        it.destroy();
    }
    
    HashMap<String, PIRLocation> &out = env().create<HashMap<String, PIRLocation>>();
    {
        Iterator<String> &it = pasmInst.out.keys();
        while (it.hasNext()) {
            String &reg = it.next();
            ExpressionNode &ex = pasmInst.out.get(reg);
            ex.accept(*this);
            if ((lastLocations.size() == 1) && (lastLocations.first()->kind == loc_temp)) {
                out.set(reg, *lastLocations.first());
                lastLocations.clear();
            } else {
                crit() << "unexpected output expression " << ex << " in " << pasmInst << "\n";
                return false;
            }
        }
        it.destroy();
    }

    curMethod->addAsm(pasmInst.pasm, in, out);
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
        } else if (lastValue) {
            PIRLocation &tmp = curMethod->newTemp(*ex.resolvedType);
            curMethod->addAssign(*lastValue, tmp);
            curMethod->addMove(tmp, *curMethod->getRet(idx++));
            lastValue = 0;
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
            curMethod->addMove(curMethod->asTemp(*lastLocations.first()), *dest, variableInit.reinterpret);
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

bool PIRGenerator::visit(ArithAssignmentExprNode & arithAssignment) {
    PIRLocation *val = 0;
    arithAssignment.value.accept(*this);
    if (lastLocations.size() == 1) {
        val = &curMethod->asTemp(*lastLocations.first());
        lastLocations.clear();
    } else if (lastValue) {
        val = &curMethod->newTemp(*arithAssignment.resolvedType);
        curMethod->addAssign(*lastValue, *val);
        lastValue = 0;
    } else {
        crit() << "unexpected value " << arithAssignment.value << " in " << arithAssignment << "\n";
        return false;
    }

    if (arithAssignment.variable.context) {
        PIRLocation *ctx = 0;
        arithAssignment.variable.context->accept(*this);
        if (lastLocations.size() == 1) {
            ctx = &curMethod->asTemp(*lastLocations.first());
            lastLocations.clear();
        } else {
            crit() << "unexpected context " << *arithAssignment.variable.context << " in " << arithAssignment << "\n";
            return false;
        }
        
        PIRLocation *tmp = &curMethod->newTemp(*arithAssignment.resolvedType);
        curMethod->addGet(*ctx, *arithAssignment.variable.resolvedVariable, *tmp);
        curMethod->addArithOp(arithAssignment.op, *tmp, *val, *tmp);
        curMethod->addSet(*ctx, *arithAssignment.variable.resolvedVariable, *tmp);
        lastLocations.add(*tmp);
    } else {
        PIRLocation *var = arithAssignment.variable.resolvedVariable->pir;
        curMethod->addArithOp(arithAssignment.op, *var, *val, *var);
        lastLocations.add(*var);
    }
    return true;
}

bool PIRGenerator::visit(ArithBinaryExprNode & arithBinary) {
    PIRLocation *left = 0;
    arithBinary.left.accept(*this);
    if (lastLocations.size() == 1) {
        left = &curMethod->asTemp(*lastLocations.first());
        lastLocations.clear();
    } else if (lastValue) {
        left = &curMethod->newTemp(*arithBinary.resolvedType);
        curMethod->addAssign(*lastValue, *left);
        lastValue = 0;
    } else {
        crit() << "unexpected left " << arithBinary.left << " in " << arithBinary << "\n";
        return false;
    }
    PIRLocation *right = 0;
    arithBinary.right.accept(*this);
    if (lastLocations.size() == 1) {
        right = &curMethod->asTemp(*lastLocations.first());
        lastLocations.clear();
    } else if (lastValue) {
        right = &curMethod->newTemp(*arithBinary.resolvedType);
        curMethod->addAssign(*lastValue, *right);
        lastValue = 0;
    } else {
        crit() << "unexpected right " << arithBinary.right << " in " << arithBinary << "\n";
        return false;
    }
    PIRLocation *dest = &curMethod->newTemp(*arithBinary.resolvedType);
    curMethod->addArithOp(arithBinary.op, *left, *right, *dest);
    lastLocations.add(*dest);
    return true;
}

bool PIRGenerator::visit(ArithUnaryExprNode & arithUnary) {
    if (arithUnary.variable.context) {
        PIRLocation *ctx = 0;
        arithUnary.variable.context->accept(*this);
        if (lastLocations.size() == 1) {
            ctx = &curMethod->asTemp(*lastLocations.first());
            lastLocations.clear();
        } else {
            crit() << "unexpected context " << *arithUnary.variable.context << " in " << arithUnary << "\n";
            return false;
        }
        PIRLocation *tmp = &curMethod->newTemp(*arithUnary.resolvedType);
        curMethod->addGet(*ctx, *arithUnary.variable.resolvedVariable, *tmp);
        switch (arithUnary.op) {
            case unary_inc: {
                curMethod->addArithOp(op_add, *tmp, *curMethod->getOneTemp(*arithUnary.resolvedType), *tmp);
                curMethod->addSet(*ctx, *arithUnary.variable.resolvedVariable, *tmp);
                break;
            }
            case unary_dec: {
                curMethod->addArithOp(op_sub, *tmp, *curMethod->getOneTemp(*arithUnary.resolvedType), *tmp);
                curMethod->addSet(*ctx, *arithUnary.variable.resolvedVariable, *tmp);
                break;
            }
            case unary_post_inc: {
                PIRLocation *tmp2 = &curMethod->newTemp(*arithUnary.resolvedType);
                curMethod->addArithOp(op_add, *tmp, *curMethod->getOneTemp(*arithUnary.resolvedType), *tmp2);
                curMethod->addSet(*ctx, *arithUnary.variable.resolvedVariable, *tmp2);
                break;
            }
            case unary_post_dec: {
                PIRLocation *tmp2 = &curMethod->newTemp(*arithUnary.resolvedType);
                curMethod->addArithOp(op_sub, *tmp, *curMethod->getOneTemp(*arithUnary.resolvedType), *tmp2);
                curMethod->addSet(*ctx, *arithUnary.variable.resolvedVariable, *tmp2);
                break;
            }
        }
        lastLocations.add(*tmp);
    } else {
        PIRLocation *var = arithUnary.variable.resolvedVariable->pir;
        switch (arithUnary.op) {
            case unary_inc: {
                curMethod->addArithOp(op_add, *var, *curMethod->getOneTemp(*arithUnary.resolvedType), *var);
                lastLocations.add(*var);
                break;
            }
            case unary_dec: {
                curMethod->addArithOp(op_sub, *var, *curMethod->getOneTemp(*arithUnary.resolvedType), *var);
                lastLocations.add(*var);
                break;
            }
            case unary_post_inc: {
                PIRLocation &tmp = curMethod->newTemp(*arithUnary.resolvedType);
                curMethod->addMove(*var, tmp);
                curMethod->addArithOp(op_add, tmp, *curMethod->getOneTemp(*arithUnary.resolvedType), *var);
                lastLocations.add(tmp);
                break;
            }
            case unary_post_dec: {
                PIRLocation &tmp = curMethod->newTemp(*arithUnary.resolvedType);
                curMethod->addMove(*var, tmp);
                curMethod->addArithOp(op_sub, tmp, *curMethod->getOneTemp(*arithUnary.resolvedType), *var);
                lastLocations.add(tmp);
                break;
            }
        }
    }
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
        } else if (lastValue) {
            PIRLocation &tmp = curMethod->newTemp(*assignment.resolvedType);
            curMethod->addAssign(*lastValue, tmp);
            curMethod->addSet(*ctx, *assignment.variable.resolvedVariable, tmp);
            lastValue = 0;
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

bool PIRGenerator::visit(NullExprNode & constNull) {
    lastValue = &curMethod->getNull();
    return true;
}

bool PIRGenerator::visit(SignExprNode & sign) {
    sign.expression.accept(*this);
    if (lastLocations.size() == 1) {
        switch (sign.sign) {
            case sign_plus:
                return true;
            case sign_minus:
                PIRLocation &left = *curMethod->getZeroTemp(*sign.expression.resolvedType);
                PIRLocation &dest = curMethod->newTemp(*sign.expression.resolvedType);
                
                curMethod->addArithOp(op_sub, left, curMethod->asTemp(*lastLocations.first()), dest);
                lastLocations.clear();
                lastLocations.add(dest);
                return true;
        }
    } else {
        crit() << "unexpected expression " << sign.expression << " in " << sign << "\n";
        return false;
    }
    return true;
}

bool PIRGenerator::visit(ThisExprNode & constThis) {
    lastLocations.add(*curMethod->getThis());
    return true;
}

bool PIRGenerator::visit(VariableExprNode & variable) {
    if (ExpressionNode *initializer = variable.resolvedVariable->finalInitializer) {
        if (variable.resolvedVariable->parent->isClass()) {
            if (ConstCStringExprNode *cCString = initializer->isConstCString()) {
                return visit(*cCString);
            }
            if (ConstIntExprNode *cInt = initializer->isConstInt()) {
                return visit(*cInt);
            }
        }
    }
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
    } else if (variable.resolvedVariable->pir) {
        lastLocations.add(*variable.resolvedVariable->pir);
    } else {
        crit() << "PIR generation: unvisited resolved variable " << *variable.resolvedVariable->getVariableDeclNode() << " found for " <<variable << "!\n";
        return false;
    }
    return true;
}
