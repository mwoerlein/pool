#ifndef POOLC_AST_VISITORS_METHODRESOLVER_HPP_LOCK
#define POOLC_AST_VISITORS_METHODRESOLVER_HPP_LOCK

#include "poolc/ast/Visitor.hpp"

#include "poolc/ast/Scope.hpp"

class MethodResolver: public Visitor {
    private:
    Scope * curScope;
    
    public:
    MethodResolver(Environment &env, MemoryInfo &mi);
    virtual ~MethodResolver();
    
    virtual bool visit(TranslationUnitNode & translationUnit) override;

    virtual bool visit(ClassDeclNode & classDecl) override;
    virtual bool visit(MethodDeclNode & methodDecl) override;
    virtual bool visit(VariableDeclNode & variableDecl) override;
    
    virtual bool visit(ClassRefNode & classRef) override;
    virtual bool visit(CStringRefNode & type) override;
    virtual bool visit(IntRefNode & type) override;
    
    virtual bool visit(BlockInstNode & block) override;
    virtual bool visit(ExpressionInstNode & exprInst) override;
    virtual bool visit(InlinePasmInstNode & pasmInst) override;
    virtual bool visit(ReturnInstNode & returnInst) override;
    virtual bool visit(VariableInitInstNode & variableInit) override;
    
    virtual bool visit(AssignmentExprNode & assignment) override;
    virtual bool visit(ConstCStringExprNode & constCString) override;
    virtual bool visit(ConstIntExprNode & constInt) override;
    virtual bool visit(MethodCallExprNode & methodCall) override;
    virtual bool visit(ThisExprNode & constThis) override;
    virtual bool visit(VariableExprNode & variable) override;
    
    virtual bool registerMethods(ClassDeclNode & classDecl);
};

#endif //POOLC_AST_VISITORS_METHODRESOLVER_HPP_LOCK
