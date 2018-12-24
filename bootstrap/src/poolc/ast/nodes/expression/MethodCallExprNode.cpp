#include "poolc/ast/nodes/expression/MethodCallExprNode.hpp"

// public
MethodCallExprNode::MethodCallExprNode(Environment &env, MemoryInfo &mi, ExpressionNode &context, String &name)
        :Object(env, mi), context(context), name(name) {}
MethodCallExprNode::~MethodCallExprNode() {
    context.destroy();
    name.destroy();
}
MethodCallExprNode * MethodCallExprNode::isMethodCall() { return this; }

bool MethodCallExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void MethodCallExprNode::printDebugName(OStream & stream) {
    stream << "method-call-<" << name << ">";
}
