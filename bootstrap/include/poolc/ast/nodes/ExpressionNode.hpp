#ifndef POOLC_AST_NODES_EXPRESSIONNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSIONNODE_HPP_LOCK

#include "poolc/ast/nodes/InstructionNode.hpp"

class ExpressionNode: public InstructionNode {
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
