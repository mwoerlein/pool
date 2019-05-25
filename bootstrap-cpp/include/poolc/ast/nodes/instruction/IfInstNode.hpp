#ifndef POOLC_AST_NODES_INSTRUCTION_IFINSTNODE_HPP_LOCK
#define POOLC_AST_NODES_INSTRUCTION_IFINSTNODE_HPP_LOCK

#include "poolc/ast/nodes/InstructionNode.hpp"

#include "poolc/ast/nodes/ExpressionNode.hpp"
#include "poolc/ast/nodes/instruction/BlockInstNode.hpp"

class IfInstNode: public InstructionNode {
    public:
    ExpressionNode &condition;
    BlockInstNode &trueBlock;
    BlockInstNode &falseBlock;
    
    IfInstNode(Environment &env, MemoryInfo &mi, ExpressionNode &condition, BlockInstNode &trueBlock, BlockInstNode &falseBlock);
    virtual ~IfInstNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_INSTRUCTION_IFINSTNODE_HPP_LOCK
