#ifndef POOLC_AST_NODES_EXPRESSION_NULLEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_NULLEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"

class NullExprNode: public ExpressionNode {
    public:
    NullExprNode(Environment &env, MemoryInfo &mi);
    virtual ~NullExprNode();
    virtual NullExprNode * isNull() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_NULLEXPRNODE_HPP_LOCK
