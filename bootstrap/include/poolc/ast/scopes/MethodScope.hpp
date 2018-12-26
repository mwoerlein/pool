#ifndef POOLC_AST_SCOPES_METHODSCOPE_HPP_LOCK
#define POOLC_AST_SCOPES_METHODSCOPE_HPP_LOCK

#include "poolc/ast/Scope.hpp"

#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"

class MethodScope: public Scope {
    private:
    MethodDeclNode & methodDecl;
    
    public:
    MethodScope(Environment &env, MemoryInfo &mi, MethodDeclNode & methodDecl, InstanceScope & parent);
    virtual ~MethodScope();
    virtual MethodScope * isMethod() override;
    virtual MethodDeclNode * getMethodDeclNode() override;
};

#endif //POOLC_AST_SCOPES_METHODSCOPE_HPP_LOCK
