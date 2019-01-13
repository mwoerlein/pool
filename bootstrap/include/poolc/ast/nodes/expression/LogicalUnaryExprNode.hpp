#ifndef POOLC_AST_NODES_EXPRESSION_LOGICALUNARYEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_LOGICALUNARYEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"

#include "poolc/Arith.hpp"

class LogicalUnaryExprNode: public ExpressionNode {
    public:
    logical_unary op;
    ExpressionNode & expression;
    
    LogicalUnaryExprNode(Environment &env, MemoryInfo &mi, logical_unary op, ExpressionNode & expression);
    virtual ~LogicalUnaryExprNode();
    virtual LogicalUnaryExprNode * isLogicalUnary() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_LOGICALUNARYEXPRNODE_HPP_LOCK
