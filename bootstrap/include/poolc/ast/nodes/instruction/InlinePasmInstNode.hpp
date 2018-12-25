#ifndef POOLC_AST_NODES_INSTRUCTION_INLINEPASMINSTNODE_HPP_LOCK
#define POOLC_AST_NODES_INSTRUCTION_INLINEPASMINSTNODE_HPP_LOCK

#include "poolc/ast/nodes/InstructionNode.hpp"

class InlinePasmInstNode: public InstructionNode {
    public:
    String & pasm;
    
    InlinePasmInstNode(Environment &env, MemoryInfo &mi);
    virtual ~InlinePasmInstNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_INSTRUCTION_INLINEPASMINSTNODE_HPP_LOCK
