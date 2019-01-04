#include "poolc/ast/visitors/TypeResolver.hpp"

#include "poolc/ast/nodes/all.hpp"

#include "poolc/ast/scopes/UnitScope.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"
#include "poolc/ast/scopes/BlockScope.hpp"
#include "poolc/ast/scopes/VariableScope.hpp"

// public
TypeResolver::TypeResolver(Environment &env, MemoryInfo &mi)
        :Object(env, mi), LoggerAware(env, mi) {}
TypeResolver::~TypeResolver() {}


bool TypeResolver::visit(TranslationUnitNode & translationUnit) {
    crit() << translationUnit.name << ": TypeResolver only works for classes, not for translation units!\n";
    return false;
}

bool TypeResolver::visit(ClassDeclNode & classDecl) {
    if (!classDecl.scope) {
        crit() << classDecl.name << ": classes must be resolved before methods!\n";
        return false;
    }
    
    ClassScope *classScope = classDecl.scope->isClass();
    if (!classScope->methodsRegistered) {
        crit() << classDecl.name << ": methods/variables must be registered before type checking/resolution!\n";
        return false;
    }
    if (classScope->typesResolved) {
        // skip already resolved class
        return true;
    }
    classScope->typesResolved = true;
    classDecl.consts.acceptAll(*this);
    classDecl.variables.acceptAll(*this);
    classDecl.methods.acceptAll(*this);
    return true;
}

bool TypeResolver::visit(MethodDeclNode & methodDecl) {
    {
        Iterator<TypeRefNode> &it = methodDecl.returnTypes.iterator();
        int idx = 0;
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
    methodDecl.parameters.acceptAll(*this);
    methodDecl.body.accept(*this);
    return true;
}

bool TypeResolver::visit(VariableDeclNode & variableDecl) {
    variableDecl.type.accept(*this);
    variableDecl.resolvedType = variableDecl.type.resolvedType;
    if (ClassScope * classScope = variableDecl.resolvedType->isClass()) {
        variableDecl.resolvedType = classScope->getInstance();
    }
    return true;
}

bool TypeResolver::visit(ClassRefNode & classRef) {
    ClassScope *scope = classRef.resolvedType->isClass();
    if (scope && !scope->typesResolved) {
        scope->getClassDeclNode()->accept(*this);
    }
    return true;
}

bool TypeResolver::visit(CStringRefNode & type) {
    return true;
}

bool TypeResolver::visit(IntRefNode & type) {
    return true;
}

bool TypeResolver::visit(BlockInstNode & block) {
    block.instructions.acceptAll(*this);
    return true;
}

bool TypeResolver::visit(ExpressionInstNode & expressionInst) {
    expressionInst.expression.accept(*this);
    return true;
}

bool TypeResolver::visit(InlinePasmInstNode & pasmInstruction) {
    return true;
}

bool TypeResolver::visit(ReturnInstNode & returnInst) {
    returnInst.values.acceptAll(*this);
    // TODO: compare values.resolvedTypes with method resultTypes
    return true;
}

bool TypeResolver::visit(VariableInitInstNode & variableInit) {
    variableInit.variables.acceptAll(*this);
    variableInit.initializer.accept(*this);
    // TODO: compare initializer.resolvedType with variables.resolvedType
    return true;
}

bool TypeResolver::visit(AssignmentExprNode & assignment) {
    assignment.variable.accept(*this);
    assignment.value.accept(*this);
    // TODO: compare variable.resolvedType with value.resolvedType
    assignment.resolvedType = assignment.variable.resolvedType;
    return true;
}

bool TypeResolver::visit(ConstCStringExprNode & constCString) {
    return true;
}

bool TypeResolver::visit(ConstIntExprNode & constInt) {
    return true;
}

bool TypeResolver::visit(MethodCallExprNode & methodCall) {
    methodCall.context.accept(*this);
    methodCall.parameters.acceptAll(*this);
    if (InstanceScope * contextInstanceScope = methodCall.context.resolvedType->isInstance()) {
        if (MethodScope *calledMethodScope = contextInstanceScope->getMethod(methodCall)) {
            methodCall.resolvedMethod = calledMethodScope;
            // TODO: compare parameters.resolvedType with calledMethod.parameters.resolvedType
        } else {
            error() << methodCall.scope->getClassDeclNode()->name 
                << ": unknown method '" << methodCall.name << "' in class '"
                << contextInstanceScope->getClassDeclNode()->name << "'\n";
            return false;
        }
    } else {
        // TODO: handle global method calls
        error() << methodCall.scope->getClassDeclNode()->name << ": invalid method call context type '" << methodCall.context.resolvedType << "'\n";
        return false;
    }
    return true;
}

bool TypeResolver::visit(ThisExprNode & constThis) {
    constThis.resolvedType = constThis.scope->getInstance();
    return true;
}

bool TypeResolver::visit(VariableExprNode & variable) {
    Scope *contextScope = variable.scope;
    if (variable.context) {
        variable.context->accept(*this);
        if (InstanceScope * contextInstanceScope = variable.context->resolvedType->isInstance()) {
            contextScope = contextInstanceScope;
        } else {
            // TODO: handle global variable/constant access
            error() << variable.scope->getClassDeclNode()->name << ": invalid variable access context type '" << variable.context->resolvedType << "'\n";
            return false;
        }
    }
    if (variable.resolvedVariable = contextScope->getVariable(variable.name)) {
        VariableDeclNode &decl = *variable.resolvedVariable->getVariableDeclNode();
        variable.resolvedType = decl.resolvedType;
        
        // convert implicit "this" to explicit "this"
        if (!variable.context && decl.scope->parent->isInstance()) {
            variable.context = &env().create<ThisExprNode>();
            variable.context->scope = variable.scope;
            variable.context->accept(*this);
        }
    } else {
        variable.printDebugInfo(error() << variable.scope->getClassDeclNode()->fullQualifiedName); 
        error() << ": unknown variable '" << variable.name << "'\n";
        return false;
    }
    return true;
}
