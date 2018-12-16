#ifndef POOLC_AST_VISITOR_HPP_LOCK
#define POOLC_AST_VISITOR_HPP_LOCK

#include "sys/Object.hpp"

class TranslationUnitNode;

class NamespaceDefNode;
class UseStatementNode;
class ClassDefNode;

class ClassRefNode;
class CStringConstDefNode;
class IntConstDefNode;
class MethodDefNode;
class MethodRefNode;
class VariableDefNode;

class InlinePasmInstructionNode;

class Visitor: virtual public Object {
    public:
    virtual ~Visitor();
    
    virtual bool visit(TranslationUnitNode & translationUnit) = 0;

    virtual bool visit(NamespaceDefNode & namespaceDef) = 0;
    virtual bool visit(UseStatementNode & useStmt) = 0;
    virtual bool visit(ClassDefNode & classDef) = 0;
    
    virtual bool visit(ClassRefNode & classRef) = 0;
    virtual bool visit(CStringConstDefNode & constDef) = 0;
    virtual bool visit(IntConstDefNode & constDef) = 0;
    virtual bool visit(VariableDefNode & variableDef) = 0;
    virtual bool visit(MethodRefNode & methodRef) = 0;
    virtual bool visit(MethodDefNode & methodDef) = 0;
    
    virtual bool visit(InlinePasmInstructionNode & pasmInstruction) = 0;
};

#endif //POOLC_AST_VISITOR_HPP_LOCK
