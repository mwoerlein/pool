#ifndef POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
#include "sys/collection/LinkedList.hpp"

class ClassDefNode: public Node {
    public:
    String & name;
    String & fullQualifiedName;
    MutableCollection<MethodDefNode> &methods;
    
    ClassDefNode(Environment &env, MemoryInfo &mi)
            :Object(env, mi), name(env.create<String>()), fullQualifiedName(env.create<String>()), methods(env.create<LinkedList<MethodDefNode>>()) {}
    virtual ~ClassDefNode() {
        name.destroy();
        fullQualifiedName.destroy();
        methods.destroy();
    }
    
    virtual bool accept(Visitor & visitor) {
        return visitor.visit(*this);
    }
};

#endif //POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK

