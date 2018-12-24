#ifndef POOLC_AST_NODES_DECLARATION_VARIABLEDECLNODE_HPP_LOCK
#define POOLC_AST_NODES_DECLARATION_VARIABLEDECLNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

class VariableDeclNode: public Node {
    public:
    String & name;
    
    VariableDeclNode(Environment &env, MemoryInfo &mi);
    virtual ~VariableDeclNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_DECLARATION_VARIABLEDECLNODE_HPP_LOCK
