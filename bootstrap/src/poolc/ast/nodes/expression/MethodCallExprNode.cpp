#include "poolc/ast/nodes/expression/MethodCallExprNode.hpp"

// public
MethodCallExprNode::MethodCallExprNode(Environment &env, MemoryInfo &mi, ExpressionNode &context, String &name, MutableCollection<ExpressionNode> &params)
        :Object(env, mi), context(context), name(name), parameters(env.create<NodeList<ExpressionNode>>()) {
    parameters.addAll(params);
}
MethodCallExprNode::~MethodCallExprNode() {
    context.destroy();
    name.destroy();
    parameters.destroyAll();
}
MethodCallExprNode * MethodCallExprNode::isMethodCall() { return this; }

bool MethodCallExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void MethodCallExprNode::printDebugName(OStream & stream) {
    stream << "method-call-<" << name << ">";
}
