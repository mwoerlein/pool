#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"

// public
MethodDeclNode::MethodDeclNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi),
         name(env.create<String>()),
         body(env.create<NodeList<InstructionNode>>()),
         kind(normal), scope(scope_instance),
         parent(0), index(-1) {
}
MethodDeclNode::~MethodDeclNode() {
    name.destroy();
    body.destroyAll();
}

bool MethodDeclNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
