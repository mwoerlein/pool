#ifndef POOLC_AST_NODES_EXPRESSION_CONSTCSTRINGEXPRNODE_HPP_LOCK
#define POOLC_AST_NODES_EXPRESSION_CONSTCSTRINGEXPRNODE_HPP_LOCK

#include "poolc/ast/nodes/ExpressionNode.hpp"

class ConstCStringExprNode: public ExpressionNode {
    public:
    String &value;
    String *stringId;
    
    ConstCStringExprNode(Environment &env, MemoryInfo &mi, String &value);
    virtual ~ConstCStringExprNode();
    virtual ConstCStringExprNode * isConstCString() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_EXPRESSION_CONSTCSTRINGEXPRNODE_HPP_LOCK
