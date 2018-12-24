#ifndef POOLC_AST_VISITOR_HPP_LOCK
#define POOLC_AST_VISITOR_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/log/LoggerAware.hpp"

class TranslationUnitNode;

class NamespaceDeclNode;
class UseStatementNode;
class ClassDeclNode;

class ClassRefNode;
class CStringConstAssignNode;
class IntConstAssignNode;
class MethodDeclNode;
class MethodRefNode;
class VariableDeclNode;

class InlinePasmInstructionNode;

class Visitor: virtual public Object, virtual public LoggerAware {
    public:
    virtual ~Visitor();
    
    virtual bool visit(TranslationUnitNode & translationUnit) = 0;

    virtual bool visit(NamespaceDeclNode & namespaceDef) = 0;
    virtual bool visit(UseStatementNode & useStmt) = 0;
    virtual bool visit(ClassDeclNode & classDef) = 0;
    
    virtual bool visit(ClassRefNode & classRef) = 0;
    virtual bool visit(CStringConstAssignNode & constDef) = 0;
    virtual bool visit(IntConstAssignNode & constDef) = 0;
    virtual bool visit(VariableDeclNode & variableDef) = 0;
    virtual bool visit(MethodRefNode & methodRef) = 0;
    virtual bool visit(MethodDeclNode & methodDef) = 0;
    
    virtual bool visit(InlinePasmInstructionNode & pasmInstruction) = 0;
};

#endif //POOLC_AST_VISITOR_HPP_LOCK
