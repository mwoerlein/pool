#include "poolc/ast/nodes/ExpressionNode.hpp"

// public
ExpressionNode::~ExpressionNode() {}

ArithAssignmentExprNode * ExpressionNode::isArithAssignment() { return 0; }
ArithBinaryExprNode * ExpressionNode::isArithBinary() { return 0; }
ArithUnaryExprNode * ExpressionNode::isArithUnary() { return 0; }
AssignmentExprNode * ExpressionNode::isAssignment() { return 0; }
ConstCStringExprNode * ExpressionNode::isConstCString() { return 0; }
ConstIntExprNode * ExpressionNode::isConstInt() { return 0; }
MethodCallExprNode * ExpressionNode::isMethodCall() { return 0; }
NullExprNode * ExpressionNode::isNull() { return 0; }
SignExprNode * ExpressionNode::isSign() { return 0; }
ThisExprNode * ExpressionNode::isThis() { return 0; }
VariableExprNode * ExpressionNode::isVariable() { return 0; }
ClassRefNode * ExpressionNode::isClass() { return 0; }
