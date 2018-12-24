#ifndef POOLC_AST_NODES_EXPRESSION_METHODCALLEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_METHODCALLEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"

class MethodCallExprNode: public ExpressionNode {
    public:
    String &name;
    ExpressionNode &context;
    // TODO: #3 handle parameter
    
    MethodCallExprNode(Environment &env, MemoryInfo &mi, ExpressionNode &context, String &name);
    virtual ~MethodCallExprNode();
    virtual MethodCallExprNode * isMethodCall() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_METHODCALLEXPRNODE_HPP_LOCK
