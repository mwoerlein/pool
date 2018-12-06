#ifndef POOLC_AST_NODES_INSTRUCTION_INLINEPASMINSTRUCTIONNODE_HPP_LOCK
#define POOLC_AST_NODES_INSTRUCTION_INLINEPASMINSTRUCTIONNODE_HPP_LOCK

#include "poolc/ast/nodes/InstructionNode.hpp"

class InlinePasmInstructionNode: public InstructionNode {
    public:
    String & pasm;
    
    InlinePasmInstructionNode(Environment &env, MemoryInfo &mi)
            :Object(env, mi), pasm(env.create<String>()) {}
    virtual ~InlinePasmInstructionNode() {
        pasm.destroy();
    }
    
    virtual bool accept(Visitor & visitor) {
        return visitor.visit(*this);
    }
};

#endif //POOLC_AST_NODES_INSTRUCTION_INLINEPASMINSTRUCTIONNODE_HPP_LOCK

