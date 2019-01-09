#ifndef POOLC_AST_NODES_EXPRESSION_ARITHASSIGNMENTEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_ARITHASSIGNMENTEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"

class ArithAssignmentExprNode: public ExpressionNode {
    public:
    arith_op op;
    VariableExprNode & variable;
    ExpressionNode & value;
    
    ArithAssignmentExprNode(Environment &env, MemoryInfo &mi, arith_op op, VariableExprNode & variable, ExpressionNode & value);
    virtual ~ArithAssignmentExprNode();
    virtual ArithAssignmentExprNode * isArithAssignment() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_ARITHASSIGNMENTEXPRNODE_HPP_LOCK
