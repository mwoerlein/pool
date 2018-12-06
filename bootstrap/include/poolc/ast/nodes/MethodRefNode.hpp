#ifndef POOLC_AST_NODES_METHODREFNODE_HPP_LOCK
#define POOLC_AST_NODES_METHODREFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

class MethodRefNode: public Node {
    public:
    MethodDefNode & methodDef;
    ClassDefNode *parent;
    
    MethodRefNode(Environment &env, MemoryInfo &mi, MethodDefNode & methodDef);
    virtual ~MethodRefNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_METHODREFNODE_HPP_LOCK
