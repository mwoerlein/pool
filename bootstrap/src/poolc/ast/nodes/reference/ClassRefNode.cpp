#include "poolc/ast/nodes/reference/ClassRefNode.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"

// public
ClassRefNode::ClassRefNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), name(env.create<FullQualifiedName>()), classDef(0) {}
ClassRefNode::~ClassRefNode() {
    name.destroy();
}

bool ClassRefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void ClassRefNode::printDebugName(OStream & stream) {
    stream << "class-ref-<" << name << ">";
}
