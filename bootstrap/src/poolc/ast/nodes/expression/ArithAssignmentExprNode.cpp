#include "poolc/ast/nodes/expression/ArithAssignmentExprNode.hpp"

#include "poolc/ast/nodes/expression/VariableExprNode.hpp"

// public
ArithAssignmentExprNode::ArithAssignmentExprNode(Environment &env, MemoryInfo &mi, arith_op op, VariableExprNode & variable, ExpressionNode & value)
        :Object(env, mi), op(op), variable(variable), value(value) {}
ArithAssignmentExprNode::~ArithAssignmentExprNode() {
    variable.destroy();
    value.destroy();
}
ArithAssignmentExprNode * ArithAssignmentExprNode::isArithAssignment() { return this; }

bool ArithAssignmentExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void ArithAssignmentExprNode::printDebugName(OStream & stream) {
    stream << "arith-assignment-<" << variable << ">";
}
