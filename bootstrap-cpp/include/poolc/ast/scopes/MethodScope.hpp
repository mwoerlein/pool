#ifndef POOLC_AST_SCOPES_METHODSCOPE_HPP_LOCK
#define POOLC_AST_SCOPES_METHODSCOPE_HPP_LOCK

#include "poolc/ast/Scope.hpp"

#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"

class PIRMethod;
class MethodScope: public Scope {
    private:
    MethodDeclNode & methodDecl;
    
    public:
    int index;
    bool typesResolved;
    PIRMethod *pir;
    
    MethodScope(Environment &env, MemoryInfo &mi, Scope & parent, MethodDeclNode & methodDecl);
    virtual ~MethodScope();
    virtual MethodScope * isMethod() override;
    virtual MethodScope * getMethod() override;
    virtual MethodDeclNode * getMethodDeclNode() override;

    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_SCOPES_METHODSCOPE_HPP_LOCK
