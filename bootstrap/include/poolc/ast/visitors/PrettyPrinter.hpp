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

    virtual bool visit(ClassDeclNode & classDef) override;
    virtual bool visit(MethodDeclNode & methodDef) override;
    virtual bool visit(NamespaceDeclNode & namespaceDef) override;
    virtual bool visit(VariableDeclNode & variableDef) override;
    
    virtual bool visit(AllRefNode & type) override;
    virtual bool visit(AnyRefNode & type) override;
    virtual bool visit(ClassRefNode & classRef) override;
    virtual bool visit(CStringRefNode & type) override;
    virtual bool visit(IntRefNode & type) override;
    virtual bool visit(UseStatementNode & useStmt) override;
    
    virtual bool visit(BlockInstNode & block) override;
    virtual bool visit(ExpressionInstNode & exprInst) override;
    virtual bool visit(InlinePasmInstNode & pasmInst) override;
    virtual bool visit(ReturnInstNode & returnInst) override;
    virtual bool visit(VariableInitInstNode & variableInit) override;
    
    virtual bool visit(ArithAssignmentExprNode & arithAssignment);
    virtual bool visit(ArithBinaryExprNode & arithBinary);
    virtual bool visit(ArithUnaryExprNode & arithUnary);
    virtual bool visit(AssignmentExprNode & assignment) override;
    virtual bool visit(ConstCStringExprNode & constCString) override;
    virtual bool visit(ConstIntExprNode & constInt) override;
    virtual bool visit(MethodCallExprNode & methodCall) override;
    virtual bool visit(NullExprNode & constNull) override;
    virtual bool visit(SignExprNode & sign);
    virtual bool visit(ThisExprNode & constThis) override;
    virtual bool visit(VariableExprNode & variable) override;
};

#endif //POOLC_AST_VISITORS_PRETTYPRINTER_HPP_LOCK
