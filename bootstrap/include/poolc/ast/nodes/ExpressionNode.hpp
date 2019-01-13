#ifndef POOLC_AST_NODES_EXPRESSIONNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSIONNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

class ExpressionNode: virtual public Node {
    public:
    virtual ~ExpressionNode();
    
    virtual ArithAssignmentExprNode * isArithAssignment();
    virtual ArithBinaryExprNode * isArithBinary();
    virtual ArithUnaryExprNode * isArithUnary();
    virtual AssignmentExprNode * isAssignment();
    virtual ConstCStringExprNode * isConstCString();
    virtual ConstIntExprNode * isConstInt();
    virtual LogicalBinaryExprNode * isLogicalBinary();
    virtual LogicalUnaryExprNode * isLogicalUnary();
    virtual MethodCallExprNode * isMethodCall();
    virtual NullExprNode * isNull();
    virtual SignExprNode * isSign();
    virtual ThisExprNode * isThis();
    virtual VariableExprNode * isVariable();
    virtual ClassRefNode * isClass();
};

#endif //POOLC_AST_NODES_EXPRESSIONNODE_HPP_LOCK
