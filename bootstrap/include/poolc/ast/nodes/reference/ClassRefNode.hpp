#ifndef POOLC_AST_NODES_REFERENCE_CLASSREFNODE_HPP_LOCK
#define POOLC_AST_NODES_REFERENCE_CLASSREFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

class ClassRefNode: public Node {
    public:
    FullQualifiedName & name;
    ClassDeclNode *classDef;
    
    ClassRefNode(Environment &env, MemoryInfo &mi);
    virtual ~ClassRefNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_REFERENCE_CLASSREFNODE_HPP_LOCK
