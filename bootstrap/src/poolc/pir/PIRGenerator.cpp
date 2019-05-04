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

#include "poolc/pir/statement/PIRCond.hpp"

// public
PIRGenerator::PIRGenerator(Environment &env, MemoryInfo &mi, TypeManager &types)
        :Object(env, mi), LoggerAware(env, mi), types(types),
         curMethod(0), curBlock(0), lastValue(0),
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
    PIRLocation *_this = curMethod->getThis();
    curMethod->entry->next = curBlock = &curMethod->newBasicBlock();
    methodDecl.body.accept(*this);
    if (!curBlock->next) {
        curBlock->next = curMethod->exit;
    }
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
    lastValue = 0;
    return true;
}

bool PIRGenerator::visit(InlinePasmInstNode & pasmInst) {
    HashMap<String, PIRLocation> &in = env().create<HashMap<String, PIRLocation>>();
    {
        Iterator<String> &it = pasmInst.in.keys();
        while (it.hasNext()) {
            String &reg = it.next();
            ExpressionNode &ex = pasmInst.in.get(reg);
            if (PIRLocation *tmp = exprAsTemp(ex)) {
                in.set(reg, *tmp);
            } else {
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

    curBlock->addAsm(pasmInst.pasm, in, out);
    return true;
}

bool PIRGenerator::visit(IfInstNode & ifInst) {
    PIRBasicBlock *next = &curMethod->newBasicBlock();
    PIRBasicBlock *trueBlock = &curMethod->newBasicBlock();
    PIRBasicBlock *falseBlock = &curMethod->newBasicBlock();
    
    branch(ifInst.condition, *trueBlock, *falseBlock);
    
    curBlock = trueBlock;
    ifInst.trueBlock.instructions.acceptAll(*this);
    curBlock->next = next;
    
    curBlock = falseBlock;
    ifInst.falseBlock.instructions.acceptAll(*this);
    curBlock->next = next;
    
    curBlock = next;
}

bool PIRGenerator::visit(ReturnInstNode & returnInst) {
    Iterator<ExpressionNode> &it = returnInst.values.iterator();
    int idx = 0;
    while (it.hasNext()) {
        ExpressionNode &ex = it.next();
        if (PIRLocation *tmp = exprAsTemp(ex)) {
            curBlock->addMove(*tmp, *curMethod->getRet(idx++));
        } else {
            return false;
        }
    }
    it.destroy();
    curBlock->next = curMethod->exit;
    curBlock = &curMethod->newBasicBlock();
    return true;
}

bool PIRGenerator::visit(VariableInitInstNode & variableInit) {
    variableInit.variables.acceptAll(*this);
    
    if (variableInit.variables.size() == 1) {
        if (PIRLocation *tmp = exprAsTemp(variableInit.initializer)) {
            curBlock->addMove(*tmp, *variableInit.variables.first()->scope->isVariable()->pir);
        } else {
            return false;
        }
    } else {
        variableInit.initializer.accept(*this);
        if (lastLocations.size() == variableInit.variables.size()) {
            Iterator<VariableDeclNode> &vit = variableInit.variables.iterator();
            Iterator<PIRLocation> &lit = lastLocations.iterator();
            while (vit.hasNext()) {
                PIRLocation *dest = vit.next().scope->isVariable()->pir;
                curBlock->addMove(curMethod->asTemp(lit.next()), *dest);
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

bool PIRGenerator::visit(WhileInstNode & whileInst) {
    PIRBasicBlock *next = &curMethod->newBasicBlock();
    PIRBasicBlock *body = &curMethod->newBasicBlock();
    PIRBasicBlock *test = &curMethod->newBasicBlock();
    curBlock->next = test;
    
    curBlock = test;
    branch(whileInst.condition, *body, *next);
    curBlock = body;
    whileInst.block.accept(*this);
    curBlock->next = test;
    curBlock = next;
}

bool PIRGenerator::visit(ArithAssignmentExprNode & arithAssignment) {
    PIRLocation *val = exprAsTemp(arithAssignment.value);
    if (!val) {
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
        curBlock->addGet(*ctx, *arithAssignment.variable.resolvedVariable, *tmp);
        curBlock->addArithOp(arithAssignment.op, *tmp, *val, *tmp);
        curBlock->addSet(*ctx, *arithAssignment.variable.resolvedVariable, *tmp);
        lastLocations.add(*tmp);
    } else {
        PIRLocation *var = arithAssignment.variable.resolvedVariable->pir;
        curBlock->addArithOp(arithAssignment.op, *var, *val, *var);
        lastLocations.add(*var);
    }
    return true;
}

bool PIRGenerator::visit(ArithBinaryExprNode & arithBinary) {
    PIRLocation *left = exprAsTemp(arithBinary.left);
    PIRLocation *right = exprAsTemp(arithBinary.right);
    if (!left || !right) {
        return false;
    }
    PIRLocation *dest = &curMethod->newTemp(*arithBinary.resolvedType);
    curBlock->addArithOp(arithBinary.op, *left, *right, *dest);
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
        curBlock->addGet(*ctx, *arithUnary.variable.resolvedVariable, *tmp);
        switch (arithUnary.op) {
            case unary_inc: {
                curBlock->addArithOp(op_add, *tmp, *curMethod->getOneTemp(*arithUnary.resolvedType), *tmp);
                curBlock->addSet(*ctx, *arithUnary.variable.resolvedVariable, *tmp);
                break;
            }
            case unary_dec: {
                curBlock->addArithOp(op_sub, *tmp, *curMethod->getOneTemp(*arithUnary.resolvedType), *tmp);
                curBlock->addSet(*ctx, *arithUnary.variable.resolvedVariable, *tmp);
                break;
            }
            case unary_post_inc: {
                PIRLocation *tmp2 = &curMethod->newTemp(*arithUnary.resolvedType);
                curBlock->addArithOp(op_add, *tmp, *curMethod->getOneTemp(*arithUnary.resolvedType), *tmp2);
                curBlock->addSet(*ctx, *arithUnary.variable.resolvedVariable, *tmp2);
                break;
            }
            case unary_post_dec: {
                PIRLocation *tmp2 = &curMethod->newTemp(*arithUnary.resolvedType);
                curBlock->addArithOp(op_sub, *tmp, *curMethod->getOneTemp(*arithUnary.resolvedType), *tmp2);
                curBlock->addSet(*ctx, *arithUnary.variable.resolvedVariable, *tmp2);
                break;
            }
        }
        lastLocations.add(*tmp);
    } else {
        PIRLocation *var = arithUnary.variable.resolvedVariable->pir;
        switch (arithUnary.op) {
            case unary_inc: {
                curBlock->addArithOp(op_add, *var, *curMethod->getOneTemp(*arithUnary.resolvedType), *var);
                lastLocations.add(*var);
                break;
            }
            case unary_dec: {
                curBlock->addArithOp(op_sub, *var, *curMethod->getOneTemp(*arithUnary.resolvedType), *var);
                lastLocations.add(*var);
                break;
            }
            case unary_post_inc: {
                PIRLocation &tmp = curMethod->newTemp(*arithUnary.resolvedType);
                curBlock->addMove(*var, tmp);
                curBlock->addArithOp(op_add, tmp, *curMethod->getOneTemp(*arithUnary.resolvedType), *var);
                lastLocations.add(tmp);
                break;
            }
            case unary_post_dec: {
                PIRLocation &tmp = curMethod->newTemp(*arithUnary.resolvedType);
                curBlock->addMove(*var, tmp);
                curBlock->addArithOp(op_sub, tmp, *curMethod->getOneTemp(*arithUnary.resolvedType), *var);
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
        if (PIRLocation *tmp = exprAsTemp(assignment.value)) {
            curBlock->addSet(*ctx, *assignment.variable.resolvedVariable, *tmp);
            lastLocations.add(*tmp);
        } else {
            return false;
        }
    } else if (PIRLocation *tmp = exprAsTemp(assignment.value)) {
        curBlock->addMove(*tmp, *assignment.variable.resolvedVariable->pir);
        lastLocations.add(*tmp);
    } else {
        return false;
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

bool PIRGenerator::visit(LogicalBinaryExprNode & logicalBinary) {
    if (PIRLocation *tmp = exprAsTemp(logicalBinary)) {
        lastLocations.add(*tmp);
        return true;
    }
    return false;
}

bool PIRGenerator::visit(LogicalUnaryExprNode & logicalUnary) {
    if (PIRLocation *tmp = exprAsTemp(logicalUnary.expression)) {
        lastLocations.add(*tmp);
        return true;
    }
    return false;
}

bool PIRGenerator::visit(MethodCallExprNode & methodCall) {
    MethodDeclNode *decl = methodCall.resolvedMethod->getMethodDeclNode();
    PIRLocation *context = 0;
    if (!decl->global) {
        methodCall.context.accept(*this);
        if (lastLocations.size() == 1) {
            context = &curMethod->asTemp(*lastLocations.first());
            lastLocations.clear();
        } else {
            crit() << "unexpected context " << methodCall.context << " " << lastLocations.size() << " for " << methodCall << "\n";
            return false;
        }
    }
    
    LinkedList<PIRLocation> &params = env().create<LinkedList<PIRLocation>>();
    {
        Iterator<ExpressionNode> &it = methodCall.parameters.iterator();
        while (it.hasNext()) {
            ExpressionNode &ex = it.next();
            if (PIRLocation *tmp = exprAsTemp(ex)) {
                params.add(*tmp);
            } else {
                return false;
            }
        }
        it.destroy();
    }
    LinkedList<PIRLocation> &rets = env().create<LinkedList<PIRLocation>>();
    {
        Iterator<Type> &it = decl->resolvedReturns.iterator();
        while (it.hasNext()) {
            PIRLocation &loc = curMethod->newTemp(it.next());
            lastLocations.add(loc);
            rets.add(loc);
        }
        it.destroy();
    }
    if (context) {
        curBlock->addCall(*context, *methodCall.resolvedMethod, params, rets);
    } else {
        curBlock->addGlobalCall(*decl->scope->getMethod(), params, rets);
    }
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
                PIRLocation &left = *curMethod->getZeroTemp(types.intType);
                PIRLocation &dest = curMethod->newTemp(types.intType);
                
                curBlock->addArithOp(op_sub, left, curMethod->asTemp(*lastLocations.first()), dest);
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
            curBlock->addGet(curMethod->asTemp(*lastLocations.first()), *variable.resolvedVariable, tmp);
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

// private

PIRLocation* PIRGenerator::exprAsTemp(ExpressionNode &expr, Type *interpretAs) {
    if (expr.isLogicalBinary() || expr.isLogicalUnary()) {
        PIRBasicBlock *next = &curMethod->newBasicBlock();
        PIRBasicBlock *trueBlock = &curMethod->newBasicBlock();
        trueBlock->next = next;
        
        PIRLocation &loc = curMethod->newTemp(types.intType);
        curBlock->addMove(*curMethod->getZeroTemp(types.intType), loc);
        trueBlock->addMove(*curMethod->getOneTemp(types.intType), loc);
        
        branch(expr, *trueBlock, *next);
        curBlock = next;
        return &loc;
    }
    
    expr.accept(*this);
    if (lastLocations.size() == 1) {
        if (interpretAs && interpretAs != &lastLocations.first()->type) {
            PIRLocation &loc = curMethod->newTemp(*interpretAs);
            curBlock->addMove(curMethod->asTemp(*lastLocations.first()), loc, true);
            lastLocations.clear();
            return &loc;
        } else {
            PIRLocation &loc = curMethod->asTemp(*lastLocations.first());
            lastLocations.clear();
            return &loc;
        }
    }
    if (lastValue) {
        PIRLocation &loc = curMethod->newTemp(interpretAs ? *interpretAs : *expr.resolvedType);
        curBlock->addAssign(*lastValue, loc, interpretAs);
        lastValue = 0;
        return &loc;
    }
    crit() << "unexpected " << expr << "\n";
    return 0;
}

void PIRGenerator::branch(ExpressionNode &expr, PIRBasicBlock &trueBlock, PIRBasicBlock &falseBlock) {
    if (LogicalUnaryExprNode *logicalUnary = expr.isLogicalUnary()) {
        switch (logicalUnary->op) {
            case unary_not: {
                branch(logicalUnary->expression, falseBlock, trueBlock);
                return;
            }
        }    
    }
    if (LogicalBinaryExprNode *logicalBinary = expr.isLogicalBinary()) {
        switch (logicalBinary->op) {
            case op_and: {
                PIRBasicBlock *nextCond = &curMethod->newBasicBlock();
                branch(logicalBinary->left, *nextCond, falseBlock);
                curBlock = nextCond;
                branch(logicalBinary->right, trueBlock, falseBlock);
                return;
            }
            case op_or: {
                PIRBasicBlock *nextCond = &curMethod->newBasicBlock();
                branch(logicalBinary->left, trueBlock, *nextCond);
                curBlock = nextCond;
                branch(logicalBinary->right, trueBlock, falseBlock);
                return;
            }
            default: {
                PIRLocation *left = exprAsTemp(logicalBinary->left);
                PIRLocation *right = exprAsTemp(logicalBinary->right);
                if (left && right) {
                    curBlock->setCondNext(logicalBinary->op, *left, *right, trueBlock, falseBlock);
                }
                return;
            }
        }
    }
    if (PIRLocation *tmp = exprAsTemp(expr, &types.intType)) {
        curBlock->setCondNext(op_neq, *tmp, *curMethod->getZeroTemp(types.intType), trueBlock, falseBlock);
    }
}

