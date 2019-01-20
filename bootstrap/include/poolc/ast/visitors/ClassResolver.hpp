#ifndef POOLC_AST_VISITORS_CLASSRESOLVER_HPP_LOCK
#define POOLC_AST_VISITORS_CLASSRESOLVER_HPP_LOCK

#include "poolc/ast/Visitor.hpp"

#include "poolc/ast/Scope.hpp"
#include "poolc/parser/ClassLoader.hpp"

class ClassResolver: public Visitor {
    private:
    Scope * curScope;
    ClassLoader & loader;
    TypeManager & types;
    
    public:
    ClassResolver(Environment &env, MemoryInfo &mi, ClassLoader & loader, TypeManager & types);
    virtual ~ClassResolver();
    
    virtual bool visit(TranslationUnitNode & translationUnit) override;

    virtual bool visit(ClassDeclNode & classDecl) override;
    virtual bool visit(MethodDeclNode & methodDecl) override;
    virtual bool visit(NamespaceDeclNode & namespaceDecl) override;
    virtual bool visit(VariableDeclNode & variableDecl) override;
    
    virtual bool visit(AllRefNode & type) override;
    virtual bool visit(AnyRefNode & type) override;
    virtual bool visit(ClassRefNode & classRef) override;
    virtual bool visit(CStringRefNode & type) override;
    virtual bool visit(IntRefNode & type) override;
    virtual bool visit(UseStatementNode & useStmt) override;
    
    virtual bool visit(BlockInstNode & block) override;
    virtual bool visit(ExpressionInstNode & exprInst) override;
    virtual bool visit(IfInstNode & ifInst) override;
    virtual bool visit(InlinePasmInstNode & pasmInst) override;
    virtual bool visit(ReturnInstNode & returnInst) override;
    virtual bool visit(VariableInitInstNode & variableInit) override;
    virtual bool visit(WhileInstNode & whileInst) override;
    
    virtual bool visit(ArithAssignmentExprNode & arithAssignment);
    virtual bool visit(ArithBinaryExprNode & arithBinary);
    virtual bool visit(ArithUnaryExprNode & arithUnary);
    virtual bool visit(AssignmentExprNode & assignment) override;
    virtual bool visit(ConstCStringExprNode & constCString) override;
    virtual bool visit(ConstIntExprNode & constInt) override;
    virtual bool visit(LogicalBinaryExprNode & logicalBinary) override;
    virtual bool visit(LogicalUnaryExprNode & logicalUnary) override;
    virtual bool visit(MethodCallExprNode & methodCall) override;
    virtual bool visit(NullExprNode & constNull) override;
    virtual bool visit(SignExprNode & sign);
    virtual bool visit(ThisExprNode & constThis) override;
    virtual bool visit(VariableExprNode & variable) override;
    
    virtual bool registerSupers(ClassDeclNode & classDecl);
};

#endif //POOLC_AST_VISITORS_CLASSRESOLVER_HPP_LOCK
