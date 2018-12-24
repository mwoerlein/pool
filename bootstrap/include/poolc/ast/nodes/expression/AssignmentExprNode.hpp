#ifndef POOLC_AST_NODES_EXPRESSION_ASSIGNMENTEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_ASSIGNMENTEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"

class AssignmentExprNode: public ExpressionNode {
    public:
    VariableExprNode & variable;
    ExpressionNode & value;
    
    AssignmentExprNode(Environment &env, MemoryInfo &mi, VariableExprNode & variable, ExpressionNode & value);
    virtual ~AssignmentExprNode();
    virtual AssignmentExprNode * isAssignment() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_ASSIGNMENTEXPRNODE_HPP_LOCK
