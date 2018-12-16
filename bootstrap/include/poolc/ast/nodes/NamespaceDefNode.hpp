#ifndef POOLC_AST_NODES_NAMESPACEDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_NAMESPACEDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "poolc/parser/token/FullQualifiedName.hpp"

class NamespaceDefNode: public Node {
    public:
    FullQualifiedName & name;
    
    NamespaceDefNode(Environment &env, MemoryInfo &mi);
    virtual ~NamespaceDefNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_NAMESPACEDEFNODE_HPP_LOCK
