#include "poolc/ast/nodes/MethodDefNode.hpp"

#include "poolc/ast/nodes/ClassDefNode.hpp"
#include "sys/collection/LinkedList.hpp"

// public
MethodDefNode::MethodDefNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi),
         name(env.create<String>()),
         body(env.create<LinkedList<InstructionNode>>()),
         virt(false),
         parent(0) {
}
MethodDefNode::~MethodDefNode() {
    name.destroy();
    {
        Iterator<InstructionNode> &it = body.iterator();
        while (it.hasNext()) { it.next().destroy(); }
        it.destroy();
        body.destroy();
    }
}

bool MethodDefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
