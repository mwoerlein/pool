#ifndef POOLC_AST_NODES_DECLARATION_VARIABLEDECLNODE_HPP_LOCK
#define POOLC_AST_NODES_DECLARATION_VARIABLEDECLNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "poolc/ast/nodes/ExpressionNode.hpp"
#include "poolc/ast/nodes/reference/TypeRefNode.hpp"

class VariableDeclNode: public Node {
    public:
    String & name;
    TypeRefNode & type;
    bool global;
    
    VariableDeclNode(Environment &env, MemoryInfo &mi, TypeRefNode & type, String &name);
    virtual ~VariableDeclNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_DECLARATION_VARIABLEDECLNODE_HPP_LOCK
