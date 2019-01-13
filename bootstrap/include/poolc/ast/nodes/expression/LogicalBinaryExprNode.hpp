#ifndef POOLC_AST_NODES_EXPRESSION_LOGICALBINARYEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_LOGICALBINARYEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"

#include "poolc/Arith.hpp"

class LogicalBinaryExprNode: public ExpressionNode {
    public:
    logical_op op;
    ExpressionNode & left;
    ExpressionNode & right;
    
    LogicalBinaryExprNode(Environment &env, MemoryInfo &mi, logical_op op, ExpressionNode & left, ExpressionNode & right);
    virtual ~LogicalBinaryExprNode();
    virtual LogicalBinaryExprNode * isLogicalBinary() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_LOGICALBINARYEXPRNODE_HPP_LOCK
