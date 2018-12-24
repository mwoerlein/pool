#include "poolc/ast/nodes/expression/AssignmentExprNode.hpp"

#include "poolc/ast/nodes/expression/VariableExprNode.hpp"

// public
AssignmentExprNode::AssignmentExprNode(Environment &env, MemoryInfo &mi, VariableExprNode & variable, ExpressionNode & value)
        :Object(env, mi), variable(variable), value(value) {}
AssignmentExprNode::~AssignmentExprNode() {
    variable.destroy();
    value.destroy();
}
AssignmentExprNode * AssignmentExprNode::isAssignment() { return this; }

bool AssignmentExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void AssignmentExprNode::printDebugName(OStream & stream) {
    stream << "assignment-<" << variable << ">";
}
