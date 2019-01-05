#include "poolc/ast/nodes/expression/NullExprNode.hpp"

// public
NullExprNode::NullExprNode(Environment &env, MemoryInfo &mi):Object(env, mi) {}
NullExprNode::~NullExprNode() {}
NullExprNode * NullExprNode::isNull() { return this; }

bool NullExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void NullExprNode::printDebugName(OStream & stream) {
    stream << "null";
}
