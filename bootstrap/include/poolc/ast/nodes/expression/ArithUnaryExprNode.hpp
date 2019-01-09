#ifndef POOLC_AST_NODES_EXPRESSION_ARITHUNARYEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_ARITHUNARYEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"

class ArithUnaryExprNode: public ExpressionNode {
    public:
    arith_unary op;
    VariableExprNode & variable;
    
    ArithUnaryExprNode(Environment &env, MemoryInfo &mi, arith_unary op, VariableExprNode & variable);
    virtual ~ArithUnaryExprNode();
    virtual ArithUnaryExprNode * isArithUnary() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_ARITHUNARYEXPRNODE_HPP_LOCK
