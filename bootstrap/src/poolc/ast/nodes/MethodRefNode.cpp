#include "poolc/ast/nodes/MethodRefNode.hpp"

#include "poolc/ast/nodes/ClassDefNode.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"

// public
MethodRefNode::MethodRefNode(Environment &env, MemoryInfo &mi, MethodDefNode & methodDef)
        :Object(env, mi), methodDef(methodDef), parent(0), index(-1) {}
MethodRefNode::~MethodRefNode() {
}

bool MethodRefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
