#include "poolc/ast/nodes/reference/AnyRefNode.hpp"

// public
AnyRefNode::AnyRefNode(Environment &env, MemoryInfo &mi):Object(env, mi) {}
AnyRefNode::~AnyRefNode() {}

bool AnyRefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void AnyRefNode::printDebugName(OStream & stream) {
    stream << "type-<ANY>";
}

AnyRefNode * AnyRefNode::isAny() { return this; }
