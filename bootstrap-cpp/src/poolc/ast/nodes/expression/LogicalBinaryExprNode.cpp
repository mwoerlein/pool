#include "poolc/ast/nodes/expression/LogicalBinaryExprNode.hpp"

// public
LogicalBinaryExprNode::LogicalBinaryExprNode(Environment &env, MemoryInfo &mi, logical_op op, ExpressionNode & left, ExpressionNode & right)
        :Object(env, mi), op(op), left(left), right(right) {}
LogicalBinaryExprNode::~LogicalBinaryExprNode() {
    left.destroy();
    right.destroy();
}
LogicalBinaryExprNode * LogicalBinaryExprNode::isLogicalBinary() { return this; }

bool LogicalBinaryExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void LogicalBinaryExprNode::printDebugName(OStream & stream) {
    stream << "logical-binary-<";
    switch (op) {
        case op_and: stream << "AND"; break;
        case op_or: stream << "OR"; break;
        case op_eq: stream << "EQ"; break;
        case op_neq: stream << "NEQ"; break;
        case op_lt: stream << "LT"; break;
        case op_le: stream << "LE"; break;
        case op_gt: stream << "GT"; break;
        case op_ge: stream << "GE"; break;
    }
    stream << ">";
}
