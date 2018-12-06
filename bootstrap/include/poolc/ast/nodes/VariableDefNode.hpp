#ifndef POOLC_AST_NODES_VARIABLEDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_VARIABLEDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "sys/String.hpp"

class VariableDefNode: public Node {
    public:
    String & name;
    
    VariableDefNode(Environment &env, MemoryInfo &mi);
    virtual ~VariableDefNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_VARIABLEDEFNODE_HPP_LOCK
