#include "poolc/ast/nodes/reference/IntRefNode.hpp"

// public
IntRefNode::IntRefNode(Environment &env, MemoryInfo &mi):Object(env, mi) {}
IntRefNode::~IntRefNode() {}

bool IntRefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void IntRefNode::printDebugName(OStream & stream) {
    stream << "type-<int>";
}

IntRefNode * IntRefNode::isInt() { return this; }
