#include "poolc/ast/nodes/declaration/NamespaceDeclNode.hpp"

// public
NamespaceDeclNode::NamespaceDeclNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), name(env.create<FullQualifiedName>()) {}
NamespaceDeclNode::~NamespaceDeclNode() {
    name.destroy();
}

bool NamespaceDeclNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void NamespaceDeclNode::printDebugName(OStream & stream) {
    stream << "namespace-decl-<" << name << ">";
}
