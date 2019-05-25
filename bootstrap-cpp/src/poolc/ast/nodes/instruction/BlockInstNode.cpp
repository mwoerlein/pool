#include "poolc/ast/nodes/instruction/BlockInstNode.hpp"

// public
BlockInstNode::BlockInstNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), instructions(env.create<NodeList<InstructionNode>>()) {}
BlockInstNode::~BlockInstNode() {
    instructions.destroyAll();
}

bool BlockInstNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void BlockInstNode::printDebugName(OStream & stream) {
    stream << "block";
}
