#ifndef POOLC_AST_NODES_METHODDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_METHODDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "sys/String.hpp"
#include "poolc/ast/collection/NodeList.hpp"
#include "poolc/ast/nodes/InstructionNode.hpp"

class MethodDefNode: public Node {
    public:
    String & name;
    bool virt;
    bool naked;
    scope_t scope;
    NodeList<InstructionNode> &body;
    ClassDefNode *parent;
    
    MethodDefNode(Environment &env, MemoryInfo &mi);
    virtual ~MethodDefNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_METHODDEFNODE_HPP_LOCK