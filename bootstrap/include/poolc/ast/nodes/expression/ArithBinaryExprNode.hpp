#ifndef POOLC_AST_NODES_EXPRESSION_ARITHBINARYEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_ARITHBINARYEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"

#include "poolc/Arith.hpp"

class ArithBinaryExprNode: public ExpressionNode {
    public:
    arith_op op;
    ExpressionNode & left;
    ExpressionNode & right;
    
    ArithBinaryExprNode(Environment &env, MemoryInfo &mi, arith_op op, ExpressionNode & left, ExpressionNode & right);
    virtual ~ArithBinaryExprNode();
    virtual ArithBinaryExprNode * isArithBinary() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_ARITHBINARYEXPRNODE_HPP_LOCK
