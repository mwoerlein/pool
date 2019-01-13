#include "poolc/ast/nodes/expression/LogicalUnaryExprNode.hpp"

// public
LogicalUnaryExprNode::LogicalUnaryExprNode(Environment &env, MemoryInfo &mi, logical_unary op, ExpressionNode & expression)
        :Object(env, mi), op(op), expression(expression) {}
LogicalUnaryExprNode::~LogicalUnaryExprNode() {
    expression.destroy();
}
LogicalUnaryExprNode * LogicalUnaryExprNode::isLogicalUnary() { return this; }

bool LogicalUnaryExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void LogicalUnaryExprNode::printDebugName(OStream & stream) {
    stream << "logical-unary-<";
    switch (op) {
        case unary_not: stream << "NOT-" << expression; break;
    }
    stream << ">";
}
