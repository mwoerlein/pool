#ifndef POOLC_AST_NODES_INTCONSTDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_INTCONSTDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "sys/String.hpp"

class IntConstDefNode: public Node {
    public:
    String & name;
    int value;
    bool global;
    
    IntConstDefNode(Environment &env, MemoryInfo &mi);
    virtual ~IntConstDefNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_INTCONSTDEFNODE_HPP_LOCK
