#ifndef POOLC_AST_NODES_DECLARATION_NAMESPACEDECLNODE_HPP_LOCK
#define POOLC_AST_NODES_DECLARATION_NAMESPACEDECLNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

class NamespaceDeclNode: virtual public Node {
    public:
    FullQualifiedName & name;
    
    NamespaceDeclNode(Environment &env, MemoryInfo &mi);
    virtual ~NamespaceDeclNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_DECLARATION_NAMESPACEDECLNODE_HPP_LOCK
