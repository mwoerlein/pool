#ifndef POOLC_AST_NODES_REFERENCE_CSTRINGREFNODE_HPP_LOCK
#define POOLC_AST_NODES_REFERENCE_CSTRINGREFNODE_HPP_LOCK

#include "poolc/ast/nodes/reference/TypeRefNode.hpp"

class CStringRefNode: public TypeRefNode {
    public:
    
    CStringRefNode(Environment &env, MemoryInfo &mi);
    virtual ~CStringRefNode();
    virtual CStringRefNode * isCString() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_REFERENCE_CSTRINGREFNODE_HPP_LOCK
