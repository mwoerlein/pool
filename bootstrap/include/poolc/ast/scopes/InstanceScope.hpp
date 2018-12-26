#ifndef POOLC_AST_SCOPES_INSTANCESCOPE_HPP_LOCK
#define POOLC_AST_SCOPES_INSTANCESCOPE_HPP_LOCK

#include "poolc/ast/Scope.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"

class InstanceScope: public Scope {
    private:
    ClassDeclNode & classDecl;
    
    public:
    InstanceScope(Environment &env, MemoryInfo &mi, ClassDeclNode & classDecl, ClassScope & parent);
    virtual ~InstanceScope();
    virtual InstanceScope * isInstance() override;
    virtual ClassDeclNode * getClassDeclNode() override;
};

#endif //POOLC_AST_SCOPES_INSTANCESCOPE_HPP_LOCK
