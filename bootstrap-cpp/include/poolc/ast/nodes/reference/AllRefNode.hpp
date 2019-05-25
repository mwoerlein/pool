#ifndef POOLC_AST_NODES_REFERENCE_ALLREFNODE_HPP_LOCK
#define POOLC_AST_NODES_REFERENCE_ALLREFNODE_HPP_LOCK

#include "poolc/ast/nodes/reference/TypeRefNode.hpp"

class AllRefNode: public TypeRefNode {
    public:
    
    AllRefNode(Environment &env, MemoryInfo &mi);
    virtual ~AllRefNode();
    virtual AllRefNode * isAll() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_REFERENCE_ALLREFNODE_HPP_LOCK
