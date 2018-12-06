#ifndef POOLC_AST_NODES_CSTRINGCONSTDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_CSTRINGCONSTDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

class CStringConstDefNode: public Node {
    public:
    String & name;
    String & value;
    
    CStringConstDefNode(Environment &env, MemoryInfo &mi)
            :Object(env, mi), name(env.create<String>()), value(env.create<String>()) {}
    virtual ~CStringConstDefNode() {
        name.destroy();
        value.destroy();
    }
    
    virtual bool accept(Visitor & visitor) {
        return visitor.visit(*this);
    }
};

#endif //POOLC_AST_NODES_CSTRINGCONSTDEFNODE_HPP_LOCK

