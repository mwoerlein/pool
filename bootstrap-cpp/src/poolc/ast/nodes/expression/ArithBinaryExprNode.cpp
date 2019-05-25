#include "poolc/ast/nodes/expression/ArithBinaryExprNode.hpp"

// public
ArithBinaryExprNode::ArithBinaryExprNode(Environment &env, MemoryInfo &mi, arith_op op, ExpressionNode & left, ExpressionNode & right)
        :Object(env, mi), op(op), left(left), right(right) {}
ArithBinaryExprNode::~ArithBinaryExprNode() {
    left.destroy();
    right.destroy();
}
ArithBinaryExprNode * ArithBinaryExprNode::isArithBinary() { return this; }

bool ArithBinaryExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void ArithBinaryExprNode::printDebugName(OStream & stream) {
    stream << "arith-binary-<";
    switch (op) {
        case op_add: stream << '+'; break;
        case op_sub: stream << '-'; break;
        case op_mul: stream << '*'; break;
        case op_div: stream << '/'; break;
        case op_mod: stream << '%'; break;
    }
    stream << ">";
}
