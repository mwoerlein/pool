#ifndef POOLC_AST_NODES_EXPRESSIONNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSIONNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

class ExpressionNode: public Node {
    public:
    virtual ~ExpressionNode();
    
    virtual AssignmentExprNode * isAssignment();
    virtual ConstCStringExprNode * isConstCString();
    virtual ConstIntExprNode * isConstInt();
    virtual MethodCallExprNode * isMethodCall();
    virtual ThisExprNode * isThis();
    virtual VariableExprNode * isVariable();
};

#endif //POOLC_AST_NODES_EXPRESSIONNODE_HPP_LOCK
