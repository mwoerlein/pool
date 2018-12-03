#ifndef POOLC_AST_NODES_METHODDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_METHODDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

class MethodDefNode: public Node {
    public:
    String & name;
    String & body;
    
    MethodDefNode(Environment &env, MemoryInfo &mi)
            :Object(env, mi), name(env.create<String>()), body(env.create<String>()) {}
    virtual ~MethodDefNode() {
        name.destroy();
        body.destroy();
    }
    
    virtual bool accept(Visitor & visitor) {
        return visitor.visit(*this);
    }
};

#endif //POOLC_AST_NODES_METHODDEFNODE_HPP_LOCK

