#include "poolc/ast/nodes/MethodDefNode.hpp"

#include "poolc/ast/nodes/ClassDefNode.hpp"

// public
MethodDefNode::MethodDefNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi),
         name(env.create<String>()),
         body(env.create<NodeList<InstructionNode>>()),
         virt(false),
         parent(0) {
}
MethodDefNode::~MethodDefNode() {
    name.destroy();
    body.destroyAll();
}

bool MethodDefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
