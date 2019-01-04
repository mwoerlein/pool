#ifndef POOLC_AST_SCOPES_INSTANCESCOPE_HPP_LOCK
#define POOLC_AST_SCOPES_INSTANCESCOPE_HPP_LOCK

#include "poolc/ast/Scope.hpp"
#include "poolc/ast/Type.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"

class InstanceScope: public Scope, public Type {
    private:
    ClassDeclNode & classDecl;
    
    public:
    InstanceScope(Environment &env, MemoryInfo &mi, Scope & parent, ClassDeclNode & classDecl);
    virtual ~InstanceScope();
    virtual InstanceScope * isInstance() override;
    virtual InstanceScope * getInstance() override;
    virtual ClassDeclNode * getClassDeclNode() override;
};

#endif //POOLC_AST_SCOPES_INSTANCESCOPE_HPP_LOCK
