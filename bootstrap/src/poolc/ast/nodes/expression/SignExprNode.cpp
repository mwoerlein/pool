#include "poolc/ast/nodes/expression/SignExprNode.hpp"

#include "poolc/ast/nodes/expression/VariableExprNode.hpp"

// public
SignExprNode::SignExprNode(Environment &env, MemoryInfo &mi, sign_t sign, ExpressionNode & expression)
        :Object(env, mi), sign(sign), expression(expression) {}
SignExprNode::~SignExprNode() {
    expression.destroy();
}
SignExprNode * SignExprNode::isSign() { return this; }

bool SignExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void SignExprNode::printDebugName(OStream & stream) {
    stream << "sign-<";
    switch (sign) {
        case sign_plus: stream << '+' << expression; break;
        case sign_minus: stream << '-' << expression; break;
    }
    stream << ">";
}
