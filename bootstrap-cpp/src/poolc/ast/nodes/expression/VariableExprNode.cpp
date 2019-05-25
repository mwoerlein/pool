#include "poolc/ast/nodes/expression/VariableExprNode.hpp"

// public
VariableExprNode::VariableExprNode(Environment &env, MemoryInfo &mi, String &name)
        :Object(env, mi), context(0), name(name), resolvedVariable(0) {}
VariableExprNode::VariableExprNode(Environment &env, MemoryInfo &mi, ExpressionNode &context, String &name)
        :Object(env, mi), context(&context), name(name), resolvedVariable(0) {}
VariableExprNode::~VariableExprNode() {
    if (context) { context->destroy(); }
    name.destroy();
}
VariableExprNode * VariableExprNode::isVariable() { return this; }

bool VariableExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void VariableExprNode::printDebugName(OStream & stream) {
    stream << "variable-<" << name << ">";
}
