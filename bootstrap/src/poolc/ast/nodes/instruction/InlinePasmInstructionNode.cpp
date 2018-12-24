#include "poolc/ast/nodes/instruction/InlinePasmInstructionNode.hpp"

// public
InlinePasmInstructionNode::InlinePasmInstructionNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), pasm(env.create<String>()) {}
InlinePasmInstructionNode::~InlinePasmInstructionNode() {
    pasm.destroy();
}

bool InlinePasmInstructionNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void InlinePasmInstructionNode::printDebugName(OStream & stream) {
    stream << "inline-pasm";
}
