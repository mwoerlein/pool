#ifndef POOLC_AST_VISITOR_HPP_LOCK
#define POOLC_AST_VISITOR_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/log/LoggerAware.hpp"

class TranslationUnitNode;

class ClassDeclNode;
class MethodDeclNode;
class NamespaceDeclNode;
class VariableDeclNode;

class ClassRefNode;
class CStringRefNode;
class IntRefNode;
class TypeRefNode;
class UseStatementNode;

class AssignmentExprNode;
class ConstCStringExprNode;
class ConstIntExprNode;
class MethodCallExprNode;
class NullExprNode;
class ThisExprNode;
class VariableExprNode;

class BlockInstNode;
class ExpressionInstNode;
class InlinePasmInstNode;
class ReturnInstNode;
class VariableInitInstNode;

class Visitor: virtual public Object, virtual public LoggerAware {
    public:
    virtual ~Visitor();
    
    virtual bool visit(TranslationUnitNode & translationUnit) = 0;

    virtual bool visit(ClassDeclNode & classDef);
    virtual bool visit(MethodDeclNode & methodDef);
    virtual bool visit(NamespaceDeclNode & namespaceDef);
    virtual bool visit(VariableDeclNode & variableDef);
    
    virtual bool visit(ClassRefNode & classRef);
    virtual bool visit(CStringRefNode & type);
    virtual bool visit(IntRefNode & type);
    virtual bool visit(UseStatementNode & useStmt);
    
    virtual bool visit(BlockInstNode & block);
    virtual bool visit(ExpressionInstNode & exprInst);
    virtual bool visit(InlinePasmInstNode & pasmInst);
    virtual bool visit(ReturnInstNode & returnInst);
    virtual bool visit(VariableInitInstNode & variableInit);
    
    virtual bool visit(AssignmentExprNode & assignment);
    virtual bool visit(ConstCStringExprNode & constCString);
    virtual bool visit(ConstIntExprNode & constInt);
    virtual bool visit(MethodCallExprNode & methodCall);
    virtual bool visit(NullExprNode & constNull);
    virtual bool visit(ThisExprNode & constThis);
    virtual bool visit(VariableExprNode & variable);
};

#endif //POOLC_AST_VISITOR_HPP_LOCK
