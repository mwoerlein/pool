#include "poolc/ast/nodes/instruction/InlinePasmInstNode.hpp"

// public
InlinePasmInstNode::InlinePasmInstNode(Environment &env, MemoryInfo &mi, NodeMap<ExpressionNode> &in, NodeMap<ExpressionNode> &out)
        :Object(env, mi), pasm(env.create<String>()), in(in), out(out) {}
InlinePasmInstNode::~InlinePasmInstNode() {
    pasm.destroy();
    in.destroyAll();
    out.destroyAll();
}

bool InlinePasmInstNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void InlinePasmInstNode::printDebugName(OStream & stream) {
    stream << "inline-pasm";
}
