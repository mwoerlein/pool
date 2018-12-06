#ifndef POOLC_AST_NODES_METHODDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_METHODDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"
#include "poolc/ast/nodes/InstructionNode.hpp"
#include "poolc/ast/nodes/ClassDefNode.hpp"
#include "sys/collection/LinkedList.hpp"

class MethodDefNode: public Node {
    public:
    String & name;
    bool virt;
    MutableCollection<InstructionNode> &body;
    ClassDefNode *parent;
    
    MethodDefNode(Environment &env, MemoryInfo &mi)
            :Object(env, mi),
             name(env.create<String>()),
             body(env.create<LinkedList<InstructionNode>>()),
             virt(false),
             parent(0) {
    }
    virtual ~MethodDefNode() {
        name.destroy();
        {
            Iterator<InstructionNode> &it = body.iterator();
            while (it.hasNext()) { it.next().destroy(); }
            it.destroy();
            body.destroy();
        }
    }
    
    virtual bool accept(Visitor & visitor) {
        return visitor.visit(*this);
    }
};

#endif //POOLC_AST_NODES_METHODDEFNODE_HPP_LOCK

