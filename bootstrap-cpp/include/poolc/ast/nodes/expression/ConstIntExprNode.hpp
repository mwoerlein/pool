#ifndef POOLC_AST_NODES_EXPRESSION_CONSTINTEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_CONSTINTEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"

class ConstIntExprNode: public ExpressionNode {
    public:
    int value;
    
    ConstIntExprNode(Environment &env, MemoryInfo &mi, int value);
    virtual ~ConstIntExprNode();
    virtual ConstIntExprNode * isConstInt() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_CONSTINTEXPRNODE_HPP_LOCK
