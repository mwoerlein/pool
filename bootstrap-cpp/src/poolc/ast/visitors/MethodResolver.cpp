#include "poolc/ast/visitors/MethodResolver.hpp"

#include "poolc/ast/nodes/all.hpp"

#include "poolc/ast/scopes/UnitScope.hpp"
#include "poolc/ast/scopes/StructScope.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"
#include "poolc/ast/scopes/BlockScope.hpp"
#include "poolc/ast/scopes/VariableScope.hpp"

// public
MethodResolver::MethodResolver(Environment &env, MemoryInfo &mi)
        :Object(env, mi), LoggerAware(env, mi), curScope(0), curInit(0) {}
MethodResolver::~MethodResolver() {}


bool MethodResolver::visit(TranslationUnitNode & translationUnit) {
    crit() << translationUnit.name << ": MethodResolver only works for classes, not for translation units!\n";
    return false;
}

bool MethodResolver::visit(ClassDeclNode & classDecl) {
    if (!classDecl.scope) {
        crit() << classDecl.name << ": classes must be resolved before methods!\n";
        return false;
    }
    
    ClassScope *classScope = classDecl.scope->isClass();
    if (classScope->methodsRegistered) {
        // skip already resolved class
        return true;
    }
    classScope->methodsRegistered = true;
    
    Scope *tmpScope = curScope;
    curScope = classScope;
    classDecl.extends.acceptAll(*this);
    registerSupers(classDecl);
    registerMethods(classDecl);
    
    classDecl.consts.acceptAll(*this);
    curScope = classDecl.instanceScope;
    classDecl.variables.acceptAll(*this);
    
    curScope = classScope;
    classDecl.methods.acceptAll(*this);
    
    TranslationUnitNode * curUnit = curScope->getUnitNode();
    if (curUnit->element.hasStringProperty("pool.bootstrap")) {
        // TODO #7: configure bootstrap with name and parameter types to generate method id
        String & bsName = curUnit->element.getStringProperty("pool.bootstrap");
        MethodScope * bsScope = classScope->getMethod(bsName);
        if (!bsScope) {
            warn() << curUnit->name << ": ignore missing global bootstrap method '" << bsName << "'\n";
            curUnit->element.unsetProperty("pool.bootstrap");
            curScope = tmpScope;
            return false;
        }
    }
    curScope = tmpScope;
    return true;
}

bool MethodResolver::visit(StructDeclNode & structDecl) {
    if (!structDecl.scope) {
        crit() << structDecl.name << ": classes must be resolved before methods!\n";
        return false;
    }
    
    StructScope *structScope = structDecl.scope->isStruct();
    if (structScope->sizeExpr->value >= 0) {
        // skip already resolved struct
        return true;
    }
    int offset = structScope->sizeExpr->value = 0;
    
    Scope *tmpScope = curScope;
    curScope = structScope;
    structDecl.consts.acceptAll(*this);
    {
        Iterator<VariableDeclNode> &it = structDecl.variables.iterator();
        while (it.hasNext()) {
            VariableDeclNode &variableDecl = it.next();
            variableDecl.accept(*this);
            variableDecl.scope->isVariable()->offset = offset;
            offset += 4; // TODO: get other sizes from variableDecl.type.resolvedType
        }
        it.destroy();
    }
    structScope->sizeExpr->value = offset;
    curScope = tmpScope;
    return true;
}

bool MethodResolver::visit(MethodDeclNode & methodDecl) {
    Scope *tmpScope = curScope;
    curScope = methodDecl.scope;
    
    {
        Iterator<TypeRefNode> &it = methodDecl.returnTypes.iterator();
        while (it.hasNext()) {
            TypeRefNode &ref = it.next();
            ref.accept(*this);
            if (ClassScope * classScope = ref.resolvedType->isClass()) {
                methodDecl.resolvedReturns.add(*classScope->getInstance());
            } else {
                methodDecl.resolvedReturns.add(*ref.resolvedType);
            }
        }
        it.destroy();
    }
    {
        Iterator<VariableDeclNode> &it = methodDecl.parameters.iterator();
        while (it.hasNext()) {
            VariableDeclNode &variableDecl = it.next();
            variableDecl.accept(*this);
            if (ClassScope * classScope = variableDecl.type.resolvedType->isClass()) {
                variableDecl.resolvedType = classScope->getInstance();
            } else {
                variableDecl.resolvedType = variableDecl.type.resolvedType;
            }
        }
        it.destroy();
    }
    methodDecl.body.accept(*this);
    
    curScope = tmpScope;
    return true;
}

bool MethodResolver::visit(VariableDeclNode & variableDecl) {
    variableDecl.scope = curScope->registerVariable(variableDecl);
    if (curScope->getBlock()) {
        curScope = variableDecl.scope; 
    }
    variableDecl.type.accept(*this);
    return true;
}

bool MethodResolver::visit(AllRefNode & type) {
    type.scope = curScope;
    return true;
}

bool MethodResolver::visit(AnyRefNode & type) {
    type.scope = curScope;
    return true;
}

bool MethodResolver::visit(ClassRefNode & classRef) {
    classRef.scope = curScope;
    if (ClassScope *classScope = classRef.resolvedType->isClass()) {
        classScope->getClassDeclNode()->accept(*this);
    }
    if (StructScope *structScope = classRef.resolvedType->isStruct()) {
        structScope->getStructDeclNode()->accept(*this);
    }
    return true;
}

bool MethodResolver::visit(CStringRefNode & type) {
    type.scope = curScope;
    return true;
}

bool MethodResolver::visit(IntRefNode & type) {
    type.scope = curScope;
    return true;
}

bool MethodResolver::visit(BlockInstNode & block) {
    Scope *tmpScope = curScope;
    curScope = block.scope = &env().create<BlockScope, Scope &, BlockInstNode &>(*tmpScope, block);
     
    block.instructions.acceptAll(*this);
    
    curScope = tmpScope;
    return true;
}

bool MethodResolver::visit(ExpressionInstNode & expressionInst) {
    expressionInst.scope = curScope;
    expressionInst.expression.accept(*this);
    return true;
}

bool MethodResolver::visit(IfInstNode & ifInst) {
    ifInst.scope = curScope;
    ifInst.condition.accept(*this);
    ifInst.trueBlock.accept(*this);
    ifInst.falseBlock.accept(*this);
    return true;
}

bool MethodResolver::visit(InlinePasmInstNode & pasmInstruction) {
    pasmInstruction.scope = curScope;
    pasmInstruction.in.acceptAll(*this);
    pasmInstruction.out.acceptAll(*this);
    return true;
}

bool MethodResolver::visit(ReturnInstNode & returnInst) {
    returnInst.scope = curScope;
    returnInst.values.acceptAll(*this);
    return true;
}

bool MethodResolver::visit(VariableInitInstNode & variableInit) {
    variableInit.scope = curScope;
    curInit = &variableInit;
    variableInit.initializer.accept(*this);
    // variable declarations *after* initializers to make them unaccessable *in* initializers
    variableInit.variables.acceptAll(*this);
    
    // mark (first) variable scope as final
    if (variableInit.final) {
        variableInit.variables.first()->scope->isVariable()->finalInitializer = &variableInit.initializer;
    }

    curInit = 0;
    return true;
}

bool MethodResolver::visit(WhileInstNode & whileInst) {
    whileInst.scope = curScope;
    whileInst.condition.accept(*this);
    whileInst.block.accept(*this);
    return true;
}

bool MethodResolver::visit(ArithAssignmentExprNode & arithAssignment) {
    arithAssignment.scope = curScope;
    arithAssignment.variable.accept(*this);
    arithAssignment.value.accept(*this);
    return true;
}

bool MethodResolver::visit(ArithBinaryExprNode & arithBinary) {
    arithBinary.scope = curScope;
    arithBinary.left.accept(*this);
    arithBinary.right.accept(*this);
    return true;
}

bool MethodResolver::visit(ArithUnaryExprNode & arithUnary) {
    arithUnary.scope = curScope;
    arithUnary.variable.accept(*this);
    return true;
}

bool MethodResolver::visit(AssignmentExprNode & assignment) {
    assignment.scope = curScope;
    assignment.variable.accept(*this);
    assignment.value.accept(*this);
    return true;
}

bool MethodResolver::visit(ConstCStringExprNode & constCString) {
    constCString.scope = curScope;
    String *id = 0;
    if (curScope->isClass() && curInit && (curInit->variables.size() == 1)) {
        id = &curInit->variables.first()->name;
    }
    constCString.stringId = &curScope->getClass()->stringId(constCString.value, id);
    return true;
}

bool MethodResolver::visit(ConstIntExprNode & constInt) {
    constInt.scope = curScope;
    return true;
}

bool MethodResolver::visit(LogicalBinaryExprNode & logicalBinary) {
    logicalBinary.scope = curScope;
    logicalBinary.left.accept(*this);
    logicalBinary.right.accept(*this);
    return true;
}

bool MethodResolver::visit(LogicalUnaryExprNode & logicalUnary) {
    logicalUnary.scope = curScope;
    logicalUnary.expression.accept(*this);
    return true;
}

bool MethodResolver::visit(MethodCallExprNode & methodCall) {
    methodCall.scope = curScope;
    methodCall.context.accept(*this);
    methodCall.parameters.acceptAll(*this);
    return true;
}

bool MethodResolver::visit(NullExprNode & constNull) {
    constNull.scope = curScope;
    return true;
}

bool MethodResolver::visit(SignExprNode & sign) {
    sign.scope = curScope;
    sign.expression.accept(*this);
    return true;
}

bool MethodResolver::visit(ThisExprNode & constThis) {
    constThis.scope = curScope;
    return true;
}

bool MethodResolver::visit(VariableExprNode & variable) {
    variable.scope = curScope;
    if (variable.context) {
        variable.context->accept(*this);
    }
    return true;
}

bool MethodResolver::registerSupers(ClassDeclNode & classDecl) {
    ClassScope *classScope = classDecl.scope->isClass();
    
    Iterator<TypeRefNode> &it = classDecl.extends.iterator();
    while (it.hasNext()) {
        TypeRefNode & type = it.next();
        if (ClassScope *extendClassScope = type.resolvedType->isClass()) {
            if (!extendClassScope->hasSuper(*extendClassScope)) {
                error() << classDecl.fullQualifiedName << ": cyclic class hierarchy detected! (unfinished " << type << ")\n";
            }
            Iterator<ClassScope> &sit = extendClassScope->supers();
            while (sit.hasNext()) {
                classScope->addSuper(sit.next());
            }
            sit.destroy();
        } else {
            error() << classDecl.fullQualifiedName << ": invalid super type '" << type << "'\n";
        }
    }
    it.destroy();
    
    classScope->addSuper(*classScope);
    return true;
}

bool MethodResolver::registerMethods(ClassDeclNode & classDecl) {
    ClassScope *classScope = classDecl.scope->isClass();
    InstanceScope *instanceScope = classDecl.instanceScope;
    
    // register super methods
    Iterator<TypeRefNode> &it = classDecl.extends.iterator();
    while (it.hasNext()) {
        TypeRefNode & type = it.next();
        if (ClassScope *extendClassScope = type.resolvedType->isClass()) {
            InstanceScope *extendInstanceScope = extendClassScope->getInstance();
            
            {
                Iterator<MethodScope> &mit = extendInstanceScope->methods();
                while (mit.hasNext()) {
                    MethodDeclNode *methodDecl = mit.next().getMethodDeclNode();
                    instanceScope->registerMethod(*methodDecl);
                }
                mit.destroy();
            }
            {
                Iterator<MethodScope> &mit = extendClassScope->methods();
                while (mit.hasNext()) {
                    MethodDeclNode *methodDecl = mit.next().getMethodDeclNode();
                    classScope->registerMethod(*methodDecl);
                }
                mit.destroy();
            }
            {
                Iterator<VariableScope> &vit = extendInstanceScope->variables();
                while (vit.hasNext()) {
                    instanceScope->registerVariable(vit.next());
                }
                vit.destroy();
            }
            {
                Iterator<VariableScope> &vit = extendClassScope->variables();
                while (vit.hasNext()) {
                    classScope->registerVariable(vit.next());
                }
                vit.destroy();
            }
        } else {
            error() << classDecl.fullQualifiedName << ": invalid extend type '" << type << "'\n";
        }
    }
    it.destroy();
    
    // register own methods at last, to overwrite super methods if required
    {
        int index = 0;
        Iterator<MethodDeclNode> &mit = classDecl.methods.iterator();
        while (mit.hasNext()) {
            MethodDeclNode &methodDecl = mit.next();
            methodDecl.scope = (methodDecl.global ? (Scope *) classScope : (Scope *) instanceScope)->registerMethod(methodDecl);
            methodDecl.index = index++;
        }
        mit.destroy();
    }
    
    // generate final method indices
    int index = 0;
    {
        Iterator<MethodScope> &mit = instanceScope->methods();
        while (mit.hasNext()) {
            mit.next().index = index++;
        }
        mit.destroy();
    }
    {
        Iterator<MethodScope> &mit = classScope->methods();
        while (mit.hasNext()) {
            mit.next().index = index++;
        }
        mit.destroy();
    }
    return true;
}
