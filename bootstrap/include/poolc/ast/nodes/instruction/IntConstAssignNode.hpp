#ifndef POOLC_AST_NODES_INSTRUCTION_INTCONSTASSIGNNODE_HPP_LOCK
#define POOLC_AST_NODES_INSTRUCTION_INTCONSTASSIGNNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

class IntConstAssignNode: public Node {
    public:
    String & name;
    int value;
    bool global;
    
    IntConstAssignNode(Environment &env, MemoryInfo &mi);
    virtual ~IntConstAssignNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_INSTRUCTION_INTCONSTASSIGNNODE_HPP_LOCK
