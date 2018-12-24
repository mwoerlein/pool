#include "poolc/ast/nodes/reference/UseStatementNode.hpp"

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

void UseStatementNode::printDebugName(OStream & stream) {
    stream << "use-stmt-<" << name << ">";
}
