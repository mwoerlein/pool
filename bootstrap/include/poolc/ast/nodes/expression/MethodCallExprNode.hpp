#ifndef POOLC_AST_NODES_EXPRESSION_METHODCALLEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_METHODCALLEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"

#include "poolc/ast/collection/NodeList.hpp"

class MethodCallExprNode: public ExpressionNode {
    public:
    String &name;
    ExpressionNode &context;
    NodeList<ExpressionNode> &parameters;
    
    MethodCallExprNode(Environment &env, MemoryInfo &mi, ExpressionNode &context, String &name, MutableCollection<ExpressionNode> &params);
    virtual ~MethodCallExprNode();
    virtual MethodCallExprNode * isMethodCall() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_METHODCALLEXPRNODE_HPP_LOCK
