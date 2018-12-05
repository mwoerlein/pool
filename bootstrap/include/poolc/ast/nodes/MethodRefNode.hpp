#ifndef POOLC_AST_NODES_METHODREFNODE_HPP_LOCK
#define POOLC_AST_NODES_METHODREFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
#include "poolc/ast/nodes/ClassDefNode.hpp"

class MethodRefNode: public Node {
    public:
    MethodDefNode & methodDef;
    ClassDefNode *parent;
    
    MethodRefNode(Environment &env, MemoryInfo &mi, MethodDefNode & methodDef)
            :Object(env, mi), methodDef(methodDef) {}
    virtual ~MethodRefNode() {
    }
    
    virtual bool accept(Visitor & visitor) {
        return visitor.visit(*this);
    }
};

#endif //POOLC_AST_NODES_METHODREFNODE_HPP_LOCK

