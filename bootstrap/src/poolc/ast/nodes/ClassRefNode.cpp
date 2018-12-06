#include "poolc/ast/nodes/ClassRefNode.hpp"

#include "poolc/ast/nodes/ClassDefNode.hpp"

// public
ClassRefNode::ClassRefNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), name(env.create<String>()), classDef(0) {}
ClassRefNode::~ClassRefNode() {
    name.destroy();
}

bool ClassRefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
