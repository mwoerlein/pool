#ifndef POOLC_AST_NODES_CLASSREFNODE_HPP_LOCK
#define POOLC_AST_NODES_CLASSREFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

class ClassDefNode;
class ClassRefNode: public Node {
    public:
    String & name;
    ClassDefNode *classDef;
    
    ClassRefNode(Environment &env, MemoryInfo &mi)
            :Object(env, mi), name(env.create<String>()), classDef(0) {}
    virtual ~ClassRefNode() {
        name.destroy();
    }
    
    virtual bool accept(Visitor & visitor) {
        return visitor.visit(*this);
    }
};
#include "poolc/ast/nodes/ClassDefNode.hpp"

#endif //POOLC_AST_NODES_CLASSREFNODE_HPP_LOCK

