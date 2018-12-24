#include "poolc/ast/nodes/reference/MethodRefNode.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"

// public
MethodRefNode::MethodRefNode(Environment &env, MemoryInfo &mi, MethodDeclNode & methodDef)
        :Object(env, mi), methodDef(methodDef), parent(0), index(-1) {}
MethodRefNode::~MethodRefNode() {
}

bool MethodRefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void MethodRefNode::printDebugName(OStream & stream) {
    stream << "method-ref-<" << methodDef.name << ">";
}
