#ifndef POOLC_AST_NODES_EXPRESSION_THISEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_THISEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"

class ThisExprNode: public ExpressionNode {
    public:
    ThisExprNode(Environment &env, MemoryInfo &mi);
    virtual ~ThisExprNode();
    virtual ThisExprNode * isThis() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_THISEXPRNODE_HPP_LOCK
