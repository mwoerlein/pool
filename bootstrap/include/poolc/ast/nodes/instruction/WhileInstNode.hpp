#ifndef POOLC_AST_NODES_INSTRUCTION_WHILEINSTNODE_HPP_LOCK
#define POOLC_AST_NODES_INSTRUCTION_WHILEINSTNODE_HPP_LOCK

#include "poolc/ast/nodes/InstructionNode.hpp"

#include "poolc/ast/nodes/ExpressionNode.hpp"
#include "poolc/ast/nodes/instruction/BlockInstNode.hpp"

class WhileInstNode: public InstructionNode {
    public:
    ExpressionNode &condition;
    BlockInstNode &block;
    
    WhileInstNode(Environment &env, MemoryInfo &mi, ExpressionNode &condition, BlockInstNode &block);
    virtual ~WhileInstNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_INSTRUCTION_WHILEINSTNODE_HPP_LOCK
