#include "poolc/ast/nodes/UseStatementNode.hpp"

// public
UseStatementNode::UseStatementNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi),
         name(env.create<FullQualifiedName>()),
         alias(env.create<String>()) {}
UseStatementNode::~UseStatementNode() {
    name.destroy();
    alias.destroy();
}

bool UseStatementNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
