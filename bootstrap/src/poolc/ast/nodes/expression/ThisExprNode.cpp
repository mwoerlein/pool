#include "poolc/ast/nodes/expression/ThisExprNode.hpp"

// public
ThisExprNode::ThisExprNode(Environment &env, MemoryInfo &mi):Object(env, mi) {}
ThisExprNode::~ThisExprNode() {}
ThisExprNode * ThisExprNode::isThis() { return this; }

bool ThisExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void ThisExprNode::printDebugName(OStream & stream) {
    stream << "this";
}
