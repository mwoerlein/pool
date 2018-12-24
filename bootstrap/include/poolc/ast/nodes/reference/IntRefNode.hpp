#ifndef POOLC_AST_NODES_REFERENCE_INTREFNODE_HPP_LOCK
#define POOLC_AST_NODES_REFERENCE_INTREFNODE_HPP_LOCK

#include "poolc/ast/nodes/reference/TypeRefNode.hpp"

class IntRefNode: public TypeRefNode {
    public:
    
    IntRefNode(Environment &env, MemoryInfo &mi);
    virtual ~IntRefNode();
    virtual IntRefNode * isInt() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_REFERENCE_INTREFNODE_HPP_LOCK
