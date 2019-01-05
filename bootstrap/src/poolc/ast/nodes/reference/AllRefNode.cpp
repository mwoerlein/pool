#include "poolc/ast/nodes/reference/AllRefNode.hpp"

// public
AllRefNode::AllRefNode(Environment &env, MemoryInfo &mi):Object(env, mi) {}
AllRefNode::~AllRefNode() {}

bool AllRefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void AllRefNode::printDebugName(OStream & stream) {
    stream << "type-<ALL>";
}

AllRefNode * AllRefNode::isAll() { return this; }
