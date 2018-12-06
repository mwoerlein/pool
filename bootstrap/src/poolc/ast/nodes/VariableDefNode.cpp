#include "poolc/ast/nodes/VariableDefNode.hpp"

// public
VariableDefNode::VariableDefNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), name(env.create<String>()) {}
VariableDefNode::~VariableDefNode() {
    name.destroy();
}

bool VariableDefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
