#include "poolc/ast/visitors/TypeResolver.hpp"

#include "poolc/ast/nodes/all.hpp"

#include "poolc/ast/scopes/UnitScope.hpp"
#include "poolc/ast/scopes/StructScope.hpp"
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
        crit() << classDecl.name << ": classes must be resolved before type checking/resolution!\n";
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

bool TypeResolver::visit(StructDeclNode & structDecl) {
    if (!structDecl.scope) {
        crit() << structDecl.name << ": classes must be resolved before type checking/resolution!\n";
        return false;
    }
    
    StructScope *structScope = structDecl.scope->isStruct();
    if (structScope->sizeExpr->value < 0) {
        crit() << structDecl.name << ": methods/variables must be registered before type checking/resolution!\n";
        return false;
    }
    if (structScope->typesResolved) {
        // skip already resolved class
        return true;
    }
    structScope->typesResolved = true;
    structDecl.consts.acceptAll(*this);
    structDecl.variables.acceptAll(*this);
    return true;
}

bool TypeResolver::visit(MethodDeclNode & methodDecl) {
    if (!methodDecl.scope) {
        crit() << methodDecl << ": classes must be resolved before type checking/resolution!\n";
        return false;
    }
    MethodScope *methodScope = methodDecl.scope->isMethod();
    if (methodScope->typesResolved) {
        return true;
    }
    methodScope->typesResolved = true;
    
    methodDecl.returnTypes.acceptAll(*this);
    methodDecl.parameters.acceptAll(*this);
    methodDecl.body.accept(*this);
    return true;
}

bool TypeResolver::visit(VariableDeclNode & variableDecl) {
    variableDecl.type.accept(*this);
    
    if (!variableDecl.resolvedType) {
        if (ClassScope * classScope = variableDecl.type.resolvedType->isClass()) {
            variableDecl.resolvedType = classScope->getInstance();
        } else {
            variableDecl.resolvedType = variableDecl.type.resolvedType;
        }
    }
    return true;
}

bool TypeResolver::visit(ClassRefNode & classRef) {
    if (ClassScope *classScope = classRef.resolvedType->isClass()) {
        if (!classScope->typesResolved) {
            classScope->getClassDeclNode()->accept(*this);
        }
    }
    if (StructScope *structScope = classRef.resolvedType->isStruct()) {
        if (!structScope->typesResolved) {
            structScope->getStructDeclNode()->accept(*this);
        }
    }
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
    pasmInstruction.in.acceptAll(*this);
    pasmInstruction.out.acceptAll(*this);
    return true;
}

bool TypeResolver::visit(IfInstNode & ifInst) {
    ifInst.condition.accept(*this);
    if (!ifInst.condition.resolvedType->isBool()) {
        ifInst.printDebugInfo(error() << ifInst.scope->getClassDeclNode()->fullQualifiedName); 
        error() << ": invalid condition '" << *ifInst.condition.resolvedType << "' in " << ifInst << "\n";
        return false;
    }
    ifInst.trueBlock.accept(*this);
    ifInst.falseBlock.accept(*this);
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
    if (variableInit.final) {
        variableInit.variables.first()->scope->isVariable()->finalInitializer = &variableInit.initializer;
    }
    return true;
}

bool TypeResolver::visit(WhileInstNode & whileInst) {
    whileInst.condition.accept(*this);
    if (!whileInst.condition.resolvedType->isBool()) {
        whileInst.printDebugInfo(error() << whileInst.scope->getClassDeclNode()->fullQualifiedName); 
        error() << ": invalid condition '" << *whileInst.condition.resolvedType << "' in " << whileInst << "\n";
        return false;
    }
    whileInst.block.accept(*this);
    return true;
}

bool TypeResolver::visit(ArithAssignmentExprNode & arithAssignment) {
    arithAssignment.variable.accept(*this);
    if (!arithAssignment.variable.resolvedType->isInt()) {
        arithAssignment.printDebugInfo(error() << arithAssignment.scope->getClassDeclNode()->fullQualifiedName); 
        error() << ": invalid variable type '" << *arithAssignment.variable.resolvedType << "' in " << arithAssignment << "\n";
        return false;
    }
    arithAssignment.value.accept(*this);
    if (!arithAssignment.value.resolvedType->isInt()) {
        arithAssignment.printDebugInfo(error() << arithAssignment.scope->getClassDeclNode()->fullQualifiedName); 
        error() << ": invalid value type '" << *arithAssignment.value.resolvedType << "' in " << arithAssignment << "\n";
        return false;
    }
    arithAssignment.resolvedType = arithAssignment.variable.resolvedType;
    return true;
}

bool TypeResolver::visit(ArithBinaryExprNode & arithBinary) {
    arithBinary.left.accept(*this);
    if (!arithBinary.left.resolvedType->isInt()) {
        arithBinary.printDebugInfo(error() << arithBinary.scope->getClassDeclNode()->fullQualifiedName); 
        error() << ": invalid left type '" << *arithBinary.left.resolvedType << "' in " << arithBinary << "\n";
        return false;
    }
    arithBinary.right.accept(*this);
    if (!arithBinary.right.resolvedType->isInt()) {
        arithBinary.printDebugInfo(error() << arithBinary.scope->getClassDeclNode()->fullQualifiedName); 
        error() << ": invalid right type '" << *arithBinary.right.resolvedType << "' in " << arithBinary << "\n";
        return false;
    }
    arithBinary.resolvedType = arithBinary.left.resolvedType;
    return true;
}

bool TypeResolver::visit(ArithUnaryExprNode & arithUnary) {
    arithUnary.variable.accept(*this);
    if (!arithUnary.variable.resolvedType->isInt()) {
        arithUnary.printDebugInfo(error() << arithUnary.scope->getClassDeclNode()->fullQualifiedName); 
        error() << ": invalid type '" << *arithUnary.variable.resolvedType << "' in " << arithUnary << "\n";
        return false;
    }
    arithUnary.resolvedType = arithUnary.variable.resolvedType;
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

bool TypeResolver::visit(LogicalBinaryExprNode & logicalBinary) {
    logicalBinary.left.accept(*this);
    logicalBinary.right.accept(*this);
    switch (logicalBinary.op) {
        case op_and:
        case op_or:
            if (!logicalBinary.left.resolvedType->isBool()) {
                logicalBinary.printDebugInfo(error() << logicalBinary.scope->getClassDeclNode()->fullQualifiedName); 
                error() << ": invalid left type '" << *logicalBinary.left.resolvedType << "' in " << logicalBinary << "\n";
                return false;
            }
            if (!logicalBinary.right.resolvedType->isBool()) {
                logicalBinary.printDebugInfo(error() << logicalBinary.scope->getClassDeclNode()->fullQualifiedName); 
                error() << ": invalid right type '" << *logicalBinary.right.resolvedType << "' in " << logicalBinary << "\n";
                return false;
            }
            // any current types are implicit bool
            break;
        default:
            if (!logicalBinary.left.resolvedType->isInt()) {
                logicalBinary.printDebugInfo(error() << logicalBinary.scope->getClassDeclNode()->fullQualifiedName); 
                error() << ": invalid left type '" << *logicalBinary.left.resolvedType << "' in " << logicalBinary << "\n";
                return false;
            }
            if (!logicalBinary.right.resolvedType->isInt()) {
                logicalBinary.printDebugInfo(error() << logicalBinary.scope->getClassDeclNode()->fullQualifiedName); 
                error() << ": invalid right type '" << *logicalBinary.right.resolvedType << "' in " << logicalBinary << "\n";
                return false;
            }
    }
    return true;
}

bool TypeResolver::visit(LogicalUnaryExprNode & logicalUnary) {
    logicalUnary.expression.accept(*this);
    if (!logicalUnary.expression.resolvedType->isBool()) {
        logicalUnary.printDebugInfo(error() << logicalUnary.scope->getClassDeclNode()->fullQualifiedName); 
        error() << ": invalid expression type '" << *logicalUnary.expression.resolvedType << "' in " << logicalUnary << "\n";
        return false;
    }
    return true;
}

bool TypeResolver::visit(MethodCallExprNode & methodCall) {
    methodCall.context.accept(*this);
    if (!methodCall.context.resolvedType) {
        error() << methodCall.scope->getClassDeclNode()->name << ": invalid method call context '" << methodCall.context << "'\n";
        return false;
    }
    methodCall.parameters.acceptAll(*this);
    if (InstanceScope * contextInstanceScope = methodCall.context.resolvedType->isInstance()) {
        if (MethodScope *calledMethodScope = contextInstanceScope->getMethod(methodCall)) {
            methodCall.resolvedMethod = calledMethodScope;
            MethodDeclNode *decl = calledMethodScope->getMethodDeclNode();
            if (decl->returnTypes.size() == 1) {
                methodCall.resolvedType = decl->resolvedReturns.first();
            }
            // TODO: handle multi return types
            // TODO: compare parameters.resolvedType with calledMethod.parameters.resolvedType
        } else {
            error() << methodCall.scope->getClassDeclNode()->name 
                << ": unknown method '" << methodCall.name << "' in class '"
                << contextInstanceScope->getClassDeclNode()->name << "'\n";
            return false;
        }
    } else if (ClassScope * contextClassScope = methodCall.context.resolvedType->isClass()) {
        if (MethodScope *calledMethodScope = contextClassScope->getMethod(methodCall)) {
            methodCall.resolvedMethod = calledMethodScope;
            MethodDeclNode *decl = calledMethodScope->getMethodDeclNode();
            if (decl->returnTypes.size() == 1) {
                methodCall.resolvedType = decl->resolvedReturns.first();
            }
            // TODO: handle multi return types
            // TODO: compare parameters.resolvedType with calledMethod.parameters.resolvedType
            
            methodCall.scope->getClass()->addGlobalRequired(*decl->scope->getClass());
        } else {
            error() << methodCall.scope->getClassDeclNode()->name 
                << ": unknown global method '" << methodCall.name << "' in class '"
                << contextClassScope->getClassDeclNode()->name << "'\n";
            return false;
        }
    } else {
        error() << methodCall.scope->getClassDeclNode()->name << ": invalid method call context type '" << *methodCall.context.resolvedType << "'\n";
        return false;
    }
    return true;
}

bool TypeResolver::visit(NullExprNode & constNull) {
    return true;
}

bool TypeResolver::visit(SignExprNode & sign) {
    sign.expression.accept(*this);
    if (!sign.expression.resolvedType->isInt()) {
        sign.printDebugInfo(error() << sign.scope->getClassDeclNode()->fullQualifiedName); 
        error() << ": invalid type '" << *sign.expression.resolvedType << "' in " << sign << "\n";
        return false;
    }
    sign.resolvedType = sign.expression.resolvedType;
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
        } else if (ClassScope * contextClassScope = variable.context->resolvedType->isClass()) {
            contextScope = contextClassScope;
        } else if (StructScope * contextStructScope = variable.context->resolvedType->isStruct()) {
            contextScope = contextStructScope;
        } else {
            // TODO: handle global variable/constant access
            error() << variable.scope->getClassDeclNode()->name << ": invalid variable access context type '" << *variable.context->resolvedType << "'\n";
            return false;
        }
    }
    if (variable.resolvedVariable = contextScope->getVariable(variable.name)) {
        VariableDeclNode &decl = *variable.resolvedVariable->getVariableDeclNode();
        // ensure variable decl to be resolved
        decl.accept(*this);
        variable.resolvedType = decl.resolvedType;
        
        // convert implicit "this" to explicit "this"
        if (!variable.context && decl.scope->parent->isInstance()) {
            variable.context = &env().create<ThisExprNode>();
            variable.context->scope = variable.scope;
            variable.context->accept(*this);
        }
    } else {
        variable.printDebugInfo(error() << variable.scope->getClassDeclNode()->fullQualifiedName); 
        error() << ": unknown variable '" << variable.name << "' in " << *contextScope << "\n";
        return false;
    }
    return true;
}
