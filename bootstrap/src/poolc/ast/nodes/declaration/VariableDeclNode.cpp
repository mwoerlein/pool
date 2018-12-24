#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"

// public
VariableDeclNode::VariableDeclNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), name(env.create<String, const char *>("UNKNOWN")) {}
VariableDeclNode::~VariableDeclNode() {
    name.destroy();
}

bool VariableDeclNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void VariableDeclNode::printDebugName(OStream & stream) {
    stream << "variable-decl-<" << name << ">";
}
