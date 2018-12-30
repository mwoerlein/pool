#ifndef POOLC_AST_NODES_EXPRESSION_VARIABLEEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_VARIABLEEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"
#include "poolc/ast/scopes/VariableScope.hpp"

class VariableExprNode: public ExpressionNode {
    public:
    String &name;
    ExpressionNode *context;
    VariableScope *resolvedVariable;
    
    VariableExprNode(Environment &env, MemoryInfo &mi, String &name);
    VariableExprNode(Environment &env, MemoryInfo &mi, ExpressionNode &context, String &name);
    virtual ~VariableExprNode();
    virtual VariableExprNode * isVariable() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_VARIABLEEXPRNODE_HPP_LOCK
