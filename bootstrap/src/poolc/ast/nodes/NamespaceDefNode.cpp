#include "poolc/ast/nodes/NamespaceDefNode.hpp"

// public
NamespaceDefNode::NamespaceDefNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), name(env.create<FullQualifiedName>()) {}
NamespaceDefNode::~NamespaceDefNode() {
    name.destroy();
}

bool NamespaceDefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
