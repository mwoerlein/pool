#include "poolc/ast/nodes/instruction/InlinePasmInstNode.hpp"

// public
InlinePasmInstNode::InlinePasmInstNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), pasm(env.create<String>()) {}
InlinePasmInstNode::~InlinePasmInstNode() {
    pasm.destroy();
}

bool InlinePasmInstNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void InlinePasmInstNode::printDebugName(OStream & stream) {
    stream << "inline-pasm";
}
