#include "poolc/ast/nodes/instruction/ExpressionInstNode.hpp"

// public
ExpressionInstNode::ExpressionInstNode(Environment &env, MemoryInfo &mi, ExpressionNode &expression)
        :Object(env, mi), expression(expression) {}
ExpressionInstNode::~ExpressionInstNode() {
    expression.destroy();
}

bool ExpressionInstNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void ExpressionInstNode::printDebugName(OStream & stream) {
    stream << "return";
}
