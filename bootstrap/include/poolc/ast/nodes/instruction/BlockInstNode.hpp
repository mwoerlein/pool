#ifndef POOLC_AST_NODES_INSTRUCTION_BLOCKINSTNODE_HPP_LOCK
#define POOLC_AST_NODES_INSTRUCTION_BLOCKINSTNODE_HPP_LOCK

#include "poolc/ast/nodes/InstructionNode.hpp"

#include "poolc/ast/collection/NodeList.hpp"

class BlockInstNode: public InstructionNode {
    public:
    NodeList<InstructionNode> &instructions;
    
    BlockInstNode(Environment &env, MemoryInfo &mi);
    virtual ~BlockInstNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_INSTRUCTION_BLOCKINSTNODE_HPP_LOCK
