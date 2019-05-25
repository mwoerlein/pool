#include "poolc/ast/visitors/ClassResolver.hpp"

#include "poolc/ast/nodes/all.hpp"

#include "poolc/ast/scopes/UnitScope.hpp"
#include "poolc/ast/scopes/StructScope.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"
#include "poolc/ast/scopes/BlockScope.hpp"
#include "poolc/ast/scopes/VariableScope.hpp"

// public
ClassResolver::ClassResolver(Environment &env, MemoryInfo &mi, ClassLoader & loader, TypeManager & types)
        :Object(env, mi), LoggerAware(env, mi), loader(loader), types(types), curScope(0) {}
ClassResolver::~ClassResolver() {}


bool ClassResolver::visit(TranslationUnitNode & translationUnit) {
    Scope *tmpScope = curScope;
    translationUnit.scope = curScope = &env().create<UnitScope, TranslationUnitNode &>(translationUnit);
    
    if (!translationUnit.ns) {
        error() << translationUnit.name << ": " << "missing namespace" << "\n";
        curScope = tmpScope;
        return false;
    }
    translationUnit.ns->accept(*this);
    
    {
        Iterator<ClassDeclNode> &it = translationUnit.classes.iterator();
        while (it.hasNext()) {
            this->registerClass(it.next());
        }
        it.destroy();
    }
    {
        Iterator<StructDeclNode> &it = translationUnit.structs.iterator();
        while (it.hasNext()) {
            this->registerStruct(it.next());
        }
        it.destroy();
    }
    
    translationUnit.uses.acceptAll(*this);
    translationUnit.classes.acceptAll(*this);
    translationUnit.structs.acceptAll(*this);
    
    curScope = tmpScope;
    return true;
}

bool ClassResolver::visit(ClassDeclNode & classDecl) {
    if (!classDecl.scope) {
        crit() << classDecl.name << ": classes must be registered before resolving!\n";
        return false;
    }

    { // TODO: move pasm lable/prefix management to correspondig writer?
        Iterator<String> &it = classDecl.fullQualifiedName.parts();
        while (it.hasNext()) {
            classDecl.globalPrefix << '_' << it.next();
        }
        it.destroy();
        (classDecl.localPrefix << '_').printuint(classDecl.fullQualifiedName.hash(), 16, 8);
    }
    
    Scope *tmpScope = curScope;
    curScope = classDecl.scope;
    classDecl.extends.acceptAll(*this);
    
    classDecl.consts.acceptAll(*this);
    classDecl.variables.acceptAll(*this);
    classDecl.methods.acceptAll(*this);

    curScope = tmpScope;
    return true;
}

bool ClassResolver::visit(StructDeclNode & structDecl) {
    if (!structDecl.scope) {
        crit() << structDecl.name << ": structs must be registered before resolving!\n";
        return false;
    }
    
    Scope *tmpScope = curScope;
    curScope = structDecl.scope;
    
    structDecl.consts.acceptAll(*this);
    structDecl.variables.acceptAll(*this);

    curScope = tmpScope;
    return true;
}

bool ClassResolver::visit(MethodDeclNode & methodDecl) {
    methodDecl.returnTypes.acceptAll(*this);
    methodDecl.parameters.acceptAll(*this);
    methodDecl.body.accept(*this);
    return true;
}

bool ClassResolver::visit(NamespaceDeclNode & namespaceDecl) {
    namespaceDecl.scope = curScope;
    return true;
}

bool ClassResolver::visit(VariableDeclNode & variableDecl) {
    variableDecl.type.accept(*this);
    return true;
}

bool ClassResolver::visit(AllRefNode & type) {
    type.resolvedType = &types.allType;
    return true;
}

bool ClassResolver::visit(AnyRefNode & type) {
    type.resolvedType = &types.anyType;
    return true;
}

bool ClassResolver::visit(ClassRefNode & classRef) {
    if (!classRef.resolvedType) {
        if (classRef.name.isFullQualified()) {
            classRef.resolvedType = loader.getNamedType(classRef.name);
        } else {
            classRef.resolvedType = curScope->getNamedType(classRef.name);
            if (!classRef.resolvedType) {
                String & fqn = env().create<String>();
                fqn << curScope->getUnitNode()->ns->name << "::" << classRef.name;
                NamedType *type = loader.getNamedType(fqn);
                if (type) {
                    classRef.resolvedType = curScope->registerNamedType(*type, classRef.name);
                }
                fqn.destroy();
            }
        }
    }
    return classRef.resolvedType;
}

bool ClassResolver::visit(CStringRefNode & type) {
    type.resolvedType = &types.cStringType;
    return true;
}

bool ClassResolver::visit(IntRefNode & type) {
    type.resolvedType = &types.intType;
    return true;
}

bool ClassResolver::visit(UseStatementNode & useStmt) {
    useStmt.scope = curScope;
    NamedType *type = loader.getNamedType(useStmt.name);
    if (type) {
        useStmt.resolvedType = curScope->registerNamedType(*type, useStmt.alias);
    }
    return useStmt.resolvedType;
}

bool ClassResolver::visit(BlockInstNode & block) {
    block.instructions.acceptAll(*this);
    return true;
}

bool ClassResolver::visit(ExpressionInstNode & expressionInst) {
    expressionInst.expression.accept(*this);
    return true;
}

bool ClassResolver::visit(IfInstNode & ifInst) {
    ifInst.condition.accept(*this);
    ifInst.trueBlock.accept(*this);
    ifInst.falseBlock.accept(*this);
    return true;
}

bool ClassResolver::visit(InlinePasmInstNode & pasmInstruction) {
    pasmInstruction.in.acceptAll(*this);
    pasmInstruction.out.acceptAll(*this);
    return true;
}

bool ClassResolver::visit(ReturnInstNode & returnInst) {
    returnInst.values.acceptAll(*this);
    return true;
}

bool ClassResolver::visit(VariableInitInstNode & variableInit) {
    variableInit.variables.acceptAll(*this);
    variableInit.initializer.accept(*this);
    return true;
}

bool ClassResolver::visit(WhileInstNode & whileInst) {
    whileInst.condition.accept(*this);
    whileInst.block.accept(*this);
    return true;
}

bool ClassResolver::visit(ArithAssignmentExprNode & arithAssignment) {
    arithAssignment.variable.accept(*this);
    arithAssignment.value.accept(*this);
    return true;
}

bool ClassResolver::visit(ArithBinaryExprNode & arithBinary) {
    arithBinary.left.accept(*this);
    arithBinary.right.accept(*this);
    return true;
}

bool ClassResolver::visit(ArithUnaryExprNode & arithUnary) {
    arithUnary.variable.accept(*this);
    return true;
}

bool ClassResolver::visit(AssignmentExprNode & assignment) {
    assignment.variable.accept(*this);
    assignment.value.accept(*this);
    return true;
}

bool ClassResolver::visit(ConstCStringExprNode & constCString) {
    constCString.resolvedType = &types.cStringType;
    return true;
}

bool ClassResolver::visit(ConstIntExprNode & constInt) {
    constInt.resolvedType = &types.intType;
    return true;
}

bool ClassResolver::visit(LogicalBinaryExprNode & logicalBinary) {
    logicalBinary.resolvedType = &types.intType;
    logicalBinary.left.accept(*this);
    logicalBinary.right.accept(*this);
    return true;
}

bool ClassResolver::visit(LogicalUnaryExprNode & logicalUnary) {
    logicalUnary.resolvedType = &types.intType;
    logicalUnary.expression.accept(*this);
    return true;
}

bool ClassResolver::visit(MethodCallExprNode & methodCall) {
    methodCall.context.accept(*this);
    methodCall.parameters.acceptAll(*this);
    return true;
}

bool ClassResolver::visit(NullExprNode & constNull) {
    constNull.resolvedType = &types.allType;
    return true;
}

bool ClassResolver::visit(SignExprNode & sign) {
    sign.expression.accept(*this);
    return true;
}

bool ClassResolver::visit(ThisExprNode & constThis) { return true; }

bool ClassResolver::visit(VariableExprNode & variable) {
    if (variable.context) {
        variable.context->accept(*this);
    }
    return true;
}

bool ClassResolver::registerClass(ClassDeclNode & classDecl) {
    if (classDecl.scope) {
        crit() << classDecl.name << ": already resolved" << "\n";
        return false;
    }
    
    TranslationUnitNode * curUnit = curScope->getUnitNode();
    classDecl.fullQualifiedName << curUnit->ns->name << "::" << classDecl.name;
    if (classDecl.fullQualifiedName != curUnit->name) {
        error() << curUnit->name << ": class name '" << classDecl.fullQualifiedName << "' does not match compilation unit\n";
        return false;
    }
    
    classDecl.scope = curScope->registerClass(classDecl, classDecl.name);
    classDecl.instanceScope = &env().create<InstanceScope, Scope &, ClassDeclNode &>(*classDecl.scope, classDecl);
    classDecl.addStringConstant(env().create<String, const char*>(CLASSNAME_ID), classDecl.fullQualifiedName);
    loader.registerClass(classDecl);
    return true;
}

bool ClassResolver::registerStruct(StructDeclNode & structDecl) {
    if (structDecl.scope) {
        crit() << structDecl.name << ": already resolved" << "\n";
        return false;
    }
    
    TranslationUnitNode * curUnit = curScope->getUnitNode();
    structDecl.fullQualifiedName << curUnit->ns->name << "::" << structDecl.name;
    if (structDecl.fullQualifiedName != curUnit->name) {
        error() << curUnit->name << ": struct name '" << structDecl.fullQualifiedName << "' does not match compilation unit\n";
        return false;
    }
    
    StructScope *scope = curScope->registerStruct(structDecl, structDecl.name);
    scope->sizeExpr = structDecl.addIntConstant(env().create<String, const char*>(SIZEOF_ID), -1).initializer.isConstInt();
    structDecl.scope = scope;
    loader.registerStruct(structDecl);
    return true;
}
