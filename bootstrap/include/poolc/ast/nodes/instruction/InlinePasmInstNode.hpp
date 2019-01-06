#ifndef POOLC_AST_NODES_INSTRUCTION_INLINEPASMINSTNODE_HPP_LOCK
#define POOLC_AST_NODES_INSTRUCTION_INLINEPASMINSTNODE_HPP_LOCK

#include "poolc/ast/nodes/InstructionNode.hpp"

#include "poolc/ast/collection/NodeMap.hpp"
#include "poolc/ast/nodes/ExpressionNode.hpp"

class InlinePasmInstNode: public InstructionNode {
    public:
    String & pasm;
    NodeMap<ExpressionNode> &in;
    NodeMap<ExpressionNode> &out;
    
    InlinePasmInstNode(Environment &env, MemoryInfo &mi, NodeMap<ExpressionNode> &in, NodeMap<ExpressionNode> &out);
    virtual ~InlinePasmInstNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_INSTRUCTION_INLINEPASMINSTNODE_HPP_LOCK
