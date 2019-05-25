#ifndef POOLC_AST_NODES_EXPRESSION_SIGNEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_SIGNEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"

#include "poolc/Arith.hpp"

class SignExprNode: public ExpressionNode {
    public:
    sign_t sign;
    ExpressionNode & expression;
    
    SignExprNode(Environment &env, MemoryInfo &mi, sign_t sign, ExpressionNode & expression);
    virtual ~SignExprNode();
    virtual SignExprNode * isSign() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_SIGNEXPRNODE_HPP_LOCK
