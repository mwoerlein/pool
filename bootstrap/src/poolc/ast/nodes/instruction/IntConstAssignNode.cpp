#include "poolc/ast/nodes/instruction/IntConstAssignNode.hpp"

// public
IntConstAssignNode::IntConstAssignNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), name(env.create<String>()), value(0), global(false) {}
IntConstAssignNode::~IntConstAssignNode() {
    name.destroy();
}

bool IntConstAssignNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
