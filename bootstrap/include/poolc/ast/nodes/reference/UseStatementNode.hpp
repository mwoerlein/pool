#ifndef POOLC_AST_NODES_REFERENCE_USESTATEMENTNODE_HPP_LOCK
#define POOLC_AST_NODES_REFERENCE_USESTATEMENTNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "poolc/ast/scopes/NamedType.hpp"

class UseStatementNode: virtual public Node {
    public:
    String & alias;
    FullQualifiedName & name;
    NamedType *resolvedType;
    
    UseStatementNode(Environment &env, MemoryInfo &mi);
    virtual ~UseStatementNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_REFERENCE_USESTATEMENTNODE_HPP_LOCK
