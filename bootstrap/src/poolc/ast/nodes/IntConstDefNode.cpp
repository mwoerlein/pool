#include "poolc/ast/nodes/IntConstDefNode.hpp"

// public
IntConstDefNode::IntConstDefNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), name(env.create<String>()), value(0), global(false) {}
IntConstDefNode::~IntConstDefNode() {
    name.destroy();
}

bool IntConstDefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
