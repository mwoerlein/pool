#ifndef POOLC_AST_NODES_INSTRUCTION_RETURNINSTNODE_HPP_LOCK
#define POOLC_AST_NODES_INSTRUCTION_RETURNINSTNODE_HPP_LOCK

#include "poolc/ast/nodes/InstructionNode.hpp"

#include "poolc/ast/collection/NodeList.hpp"
#include "poolc/ast/nodes/ExpressionNode.hpp"

class ReturnInstNode: public InstructionNode {
    public:
    NodeList<ExpressionNode> &values;
    
    ReturnInstNode(Environment &env, MemoryInfo &mi);
    ReturnInstNode(Environment &env, MemoryInfo &mi, ExpressionNode &val);
    ReturnInstNode(Environment &env, MemoryInfo &mi, MutableCollection<ExpressionNode> &vals);
    virtual ~ReturnInstNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_INSTRUCTION_RETURNINSTNODE_HPP_LOCK
