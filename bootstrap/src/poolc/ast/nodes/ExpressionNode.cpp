#include "poolc/ast/nodes/ExpressionNode.hpp"

// public
ExpressionNode::~ExpressionNode() {}

AssignmentExprNode * ExpressionNode::isAssignment() { return 0; }
ConstCStringExprNode * ExpressionNode::isConstCString() { return 0; }
ConstIntExprNode * ExpressionNode::isConstInt() { return 0; }
MethodCallExprNode * ExpressionNode::isMethodCall() { return 0; }
ThisExprNode * ExpressionNode::isThis() { return 0; }
VariableExprNode * ExpressionNode::isVariable() { return 0; }
