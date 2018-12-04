#ifndef POOLC_AST_NODES_VARIABLEDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_VARIABLEDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

class VariableDefNode: public Node {
    public:
    String & name;
    
    VariableDefNode(Environment &env, MemoryInfo &mi)
            :Object(env, mi), name(env.create<String>()) {}
    virtual ~VariableDefNode() {
        name.destroy();
    }
    
    virtual bool accept(Visitor & visitor) {
        return visitor.visit(*this);
    }
};

#endif //POOLC_AST_NODES_VARIABLEDEFNODE_HPP_LOCK

