#include "poolc/ast/nodes/expression/ConstIntExprNode.hpp"

// public
ConstIntExprNode::ConstIntExprNode(Environment &env, MemoryInfo &mi, int value):Object(env, mi), value(value) {}
ConstIntExprNode::~ConstIntExprNode() {}
ConstIntExprNode * ConstIntExprNode::isConstInt() { return this; }

bool ConstIntExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void ConstIntExprNode::printDebugName(OStream & stream) {
    stream << "const-<" << value << ">";
}
