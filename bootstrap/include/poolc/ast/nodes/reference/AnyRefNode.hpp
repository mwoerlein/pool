#ifndef POOLC_AST_NODES_REFERENCE_ANYREFNODE_HPP_LOCK
#define POOLC_AST_NODES_REFERENCE_ANYREFNODE_HPP_LOCK

#include "poolc/ast/nodes/reference/TypeRefNode.hpp"

class AnyRefNode: public TypeRefNode {
    public:
    
    AnyRefNode(Environment &env, MemoryInfo &mi);
    virtual ~AnyRefNode();
    virtual AnyRefNode * isAny() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_REFERENCE_ANYREFNODE_HPP_LOCK
