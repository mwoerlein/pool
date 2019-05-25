#ifndef POOLC_AST_SCOPES_STRUCTSCOPE_HPP_LOCK
#define POOLC_AST_SCOPES_STRUCTSCOPE_HPP_LOCK

#include "poolc/ast/Scope.hpp"
#include "poolc/ast/scopes/NamedType.hpp"

#include "poolc/ast/nodes/declaration/StructDeclNode.hpp"
#include "poolc/ast/nodes/expression/ConstIntExprNode.hpp"

class StructScope: public Scope, public NamedType {
    private:
    StructDeclNode & structDecl;
    
    public:
    bool typesResolved;
    ConstIntExprNode* sizeExpr;
    
    StructScope(Environment &env, MemoryInfo &mi, Scope & parent, StructDeclNode & structDecl);
    virtual ~StructScope();
    virtual StructScope * isStruct() override;
    virtual StructScope * getStruct() override;
    virtual StructDeclNode * getStructDeclNode() override;
    
    virtual OStream & operator >>(OStream & stream) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_SCOPES_STRUCTSCOPE_HPP_LOCK
