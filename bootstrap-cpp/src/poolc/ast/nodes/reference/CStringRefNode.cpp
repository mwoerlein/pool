#include "poolc/ast/nodes/reference/CStringRefNode.hpp"

// public
CStringRefNode::CStringRefNode(Environment &env, MemoryInfo &mi):Object(env, mi) {}
CStringRefNode::~CStringRefNode() {}

bool CStringRefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void CStringRefNode::printDebugName(OStream & stream) {
    stream << "type-<cstring>";
}

CStringRefNode * CStringRefNode::isCString() { return this; }
