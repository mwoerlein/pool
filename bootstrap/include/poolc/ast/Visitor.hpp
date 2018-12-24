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
class MethodRefNode;
class TypeRefNode;
class UseStatementNode;

class AssignmentExprNode;
class ConstCStringExprNode;
class ConstIntExprNode;
class MethodCallExprNode;
class ThisExprNode;
class VariableExprNode;

class InlinePasmInstructionNode;
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
    virtual bool visit(MethodRefNode & methodRef);
    virtual bool visit(UseStatementNode & useStmt);
    
    virtual bool visit(InlinePasmInstructionNode & pasmInstruction);
    virtual bool visit(VariableInitInstNode & variableInit);
    
    virtual bool visit(AssignmentExprNode & assignment);
    virtual bool visit(ConstCStringExprNode & constCString);
    virtual bool visit(ConstIntExprNode & constInt);
    virtual bool visit(MethodCallExprNode & methodCall);
    virtual bool visit(ThisExprNode & constThis);
    virtual bool visit(VariableExprNode & variable);
};

#endif //POOLC_AST_VISITOR_HPP_LOCK
