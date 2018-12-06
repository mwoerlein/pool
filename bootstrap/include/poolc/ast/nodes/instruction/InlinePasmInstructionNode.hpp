#ifndef POOLC_AST_NODES_INSTRUCTION_INLINEPASMINSTRUCTIONNODE_HPP_LOCK
#define POOLC_AST_NODES_INSTRUCTION_INLINEPASMINSTRUCTIONNODE_HPP_LOCK

#include "poolc/ast/nodes/InstructionNode.hpp"

#include "sys/String.hpp"

class InlinePasmInstructionNode: public InstructionNode {
    public:
    String & pasm;
    
    InlinePasmInstructionNode(Environment &env, MemoryInfo &mi);
    virtual ~InlinePasmInstructionNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_INSTRUCTION_INLINEPASMINSTRUCTIONNODE_HPP_LOCK
