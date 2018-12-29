#include "poolc/ast/visitors/MethodResolver.hpp"

#include "poolc/ast/nodes/all.hpp"

#include "poolc/ast/scopes/UnitScope.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"
#include "poolc/ast/scopes/BlockScope.hpp"
#include "poolc/ast/scopes/VariableScope.hpp"

// public
MethodResolver::MethodResolver(Environment &env, MemoryInfo &mi)
        :Object(env, mi), LoggerAware(env, mi), curScope(0) {}
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
    
    Scope *tmpScope = curScope;
    curScope = classScope;
    classDecl.extends.acceptAll(*this);
    registerMethods(classDecl);
    classScope->methodsRegistered = true;
    
    classDecl.methods.acceptAll(*this);
    classDecl.consts.acceptAll(*this);
    curScope = classDecl.instanceScope;
    classDecl.variables.acceptAll(*this);
    
    TranslationUnitNode * curUnit = curScope->getUnitNode();
    if (curUnit->element.hasStringProperty("pool.bootstrap")) {
        // TODO: #7 configure bootstrap with name and parameter types to generate method id
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

bool MethodResolver::visit(MethodDeclNode & methodDecl) {
    Scope *tmpScope = curScope;
    curScope = methodDecl.scope;
    
    methodDecl.returnTypes.acceptAll(*this);
    methodDecl.parameters.acceptAll(*this);
    methodDecl.body.accept(*this);
    
    curScope = tmpScope;
    return true;
}

bool MethodResolver::visit(VariableDeclNode & variableDecl) {
    variableDecl.scope = curScope->registerVariable(variableDecl);
    variableDecl.type.accept(*this);
    return true;
}

bool MethodResolver::visit(ClassRefNode & classRef) {
    classRef.scope = curScope;
    classRef.resolvedType->getClassDeclNode()->accept(*this);
    return true;
}

bool MethodResolver::visit(CStringRefNode & type) {
    type.scope = curScope;
    // TODO: #3 init type.resolvedType?
    return true;
}

bool MethodResolver::visit(IntRefNode & type) {
    type.scope = curScope;
    // TODO: #3 init type.resolvedType?
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

bool MethodResolver::visit(InlinePasmInstNode & pasmInstruction) {
    pasmInstruction.scope = curScope;
    return true;
}

bool MethodResolver::visit(ReturnInstNode & returnInst) {
    returnInst.scope = curScope;
    returnInst.values.acceptAll(*this);
    return true;
}

bool MethodResolver::visit(VariableInitInstNode & variableInit) {
    variableInit.scope = curScope;
    variableInit.variables.acceptAll(*this);
    variableInit.initializer.accept(*this);
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
    return true;
}

bool MethodResolver::visit(ConstIntExprNode & constInt) {
    constInt.scope = curScope;
    return true;
}

bool MethodResolver::visit(MethodCallExprNode & methodCall) {
    methodCall.scope = curScope;
    methodCall.context.accept(*this);
    methodCall.parameters.acceptAll(*this);
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

bool MethodResolver::registerMethods(ClassDeclNode & classDecl) {
    ClassScope *classScope = classDecl.scope->isClass();
    InstanceScope *instanceScope = classDecl.instanceScope;
    
    // register super methods
    Iterator<TypeRefNode> &it = classDecl.extends.iterator();
    while (it.hasNext()) {
        TypeRefNode & type = it.next();
        if (ClassRefNode *extend = type.isClass()) {
            
            ClassScope *extendClassScope = extend->resolvedType;
            InstanceScope *extendInstanceScope = extendClassScope->getClassDeclNode()->instanceScope;
            
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
