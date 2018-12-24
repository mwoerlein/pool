#ifndef POOLC_AST_NODES_REFERENCE_METHODREFNODE_HPP_LOCK
#define POOLC_AST_NODES_REFERENCE_METHODREFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

class MethodRefNode: public Node {
    public:
    MethodDeclNode & methodDef;
    ClassDeclNode *parent;
    int index;
    
    MethodRefNode(Environment &env, MemoryInfo &mi, MethodDeclNode & methodDef);
    virtual ~MethodRefNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_REFERENCE_METHODREFNODE_HPP_LOCK
