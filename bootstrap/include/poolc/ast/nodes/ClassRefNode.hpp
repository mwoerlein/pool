#ifndef POOLC_AST_NODES_CLASSREFNODE_HPP_LOCK
#define POOLC_AST_NODES_CLASSREFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "sys/String.hpp"

class ClassRefNode: public Node {
    public:
    String & name;
    ClassDefNode *classDef;
    
    ClassRefNode(Environment &env, MemoryInfo &mi);
    virtual ~ClassRefNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_CLASSREFNODE_HPP_LOCK
