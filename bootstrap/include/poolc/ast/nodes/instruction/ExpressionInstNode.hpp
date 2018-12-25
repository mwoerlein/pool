#ifndef POOLC_AST_NODES_INSTRUCTION_EXPRESSIONINSTNODE_HPP_LOCK
#define POOLC_AST_NODES_INSTRUCTION_EXPRESSIONINSTNODE_HPP_LOCK

#include "poolc/ast/nodes/InstructionNode.hpp"

#include "poolc/ast/nodes/ExpressionNode.hpp"

class ExpressionInstNode: public InstructionNode {
    public:
    ExpressionNode &expression;
    
    ExpressionInstNode(Environment &env, MemoryInfo &mi, ExpressionNode &expression);
    virtual ~ExpressionInstNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_INSTRUCTION_EXPRESSIONINSTNODE_HPP_LOCK
