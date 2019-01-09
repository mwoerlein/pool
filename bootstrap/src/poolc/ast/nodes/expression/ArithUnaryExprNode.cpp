#include "poolc/ast/nodes/expression/ArithUnaryExprNode.hpp"

#include "poolc/ast/nodes/expression/VariableExprNode.hpp"

// public
ArithUnaryExprNode::ArithUnaryExprNode(Environment &env, MemoryInfo &mi, arith_unary op, VariableExprNode & variable)
        :Object(env, mi), op(op), variable(variable) {}
ArithUnaryExprNode::~ArithUnaryExprNode() {
    variable.destroy();
}
ArithUnaryExprNode * ArithUnaryExprNode::isArithUnary() { return this; }

bool ArithUnaryExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void ArithUnaryExprNode::printDebugName(OStream & stream) {
    stream << "arith-unary-<";
    switch (op) {
        case unary_inc: stream << "++" << variable; break;
        case unary_dec: stream << "--" << variable; break;
        case unary_post_inc: stream << variable << "++"; break;
        case unary_post_dec: stream << variable << "--"; break;
    }
    stream << ">";
}
