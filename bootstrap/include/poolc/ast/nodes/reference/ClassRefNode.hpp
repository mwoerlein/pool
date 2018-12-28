#ifndef POOLC_AST_NODES_REFERENCE_CLASSREFNODE_HPP_LOCK
#define POOLC_AST_NODES_REFERENCE_CLASSREFNODE_HPP_LOCK

#include "poolc/ast/nodes/reference/TypeRefNode.hpp"

#include "poolc/ast/scopes/ClassScope.hpp"

class ClassRefNode: public TypeRefNode {
    public:
    FullQualifiedName & name;
    ClassScope *resolvedType;
    
    ClassRefNode(Environment &env, MemoryInfo &mi);
    virtual ~ClassRefNode();
    virtual ClassRefNode * isClass() override;
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_REFERENCE_CLASSREFNODE_HPP_LOCK
