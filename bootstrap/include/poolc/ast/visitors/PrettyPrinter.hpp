#ifndef POOLC_AST_VISITORS_PRETTYPRINTER_HPP_LOCK
#define POOLC_AST_VISITORS_PRETTYPRINTER_HPP_LOCK

#include "poolc/ast/visitors/Writer.hpp"

class PrettyPrinter: public Writer {
    private:
    int _indent;
    
    virtual OStream & indent();
    virtual void startBlock(OStream *line = 0);
    virtual void endBlock();
    
    public:
    PrettyPrinter(Environment &env, MemoryInfo &mi, PoolStorage &ps);
    virtual ~PrettyPrinter();
    
    virtual bool visit(TranslationUnitNode & translationUnit) override;

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
    virtual bool visit(ThisExprNode & constThis);
    virtual bool visit(VariableExprNode & variable);
};

#endif //POOLC_AST_VISITORS_PRETTYPRINTER_HPP_LOCK
