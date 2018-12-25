#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/instruction/BlockInstNode.hpp"

// public
MethodDeclNode::MethodDeclNode(Environment &env, MemoryInfo &mi, String &name)
        :Object(env, mi),
         name(name),
         body(env.create<BlockInstNode>()),
         kind(normal), scope(scope_instance),
         parent(0), index(-1) {
}
MethodDeclNode::MethodDeclNode(Environment &env, MemoryInfo &mi, String &name, BlockInstNode &body)
        :Object(env, mi),
         name(name),
         body(body),
         kind(normal), scope(scope_instance),
         parent(0), index(-1) {
}
MethodDeclNode::~MethodDeclNode() {
    name.destroy();
    body.destroy();
}

bool MethodDeclNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void MethodDeclNode::printDebugName(OStream & stream) {
    stream << "method-decl-<" << name << ">";
}
