#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"

// public
VariableDeclNode::VariableDeclNode(Environment &env, MemoryInfo &mi, TypeRefNode &type, String &name)
        :Object(env, mi), type(type), name(name), initializer(0), scope(scope_block), global(false) {}
VariableDeclNode::~VariableDeclNode() {
    type.destroy();
    name.destroy();
    if (initializer) { initializer->destroy(); }
}

bool VariableDeclNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void VariableDeclNode::printDebugName(OStream & stream) {
    stream << "variable-decl-<" << name << ">";
}
