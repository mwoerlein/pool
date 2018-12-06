#include "poolc/ast/nodes/CStringConstDefNode.hpp"

// public
CStringConstDefNode::CStringConstDefNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), name(env.create<String>()), value(env.create<String>()) {}
CStringConstDefNode::~CStringConstDefNode() {
    name.destroy();
    value.destroy();
}

bool CStringConstDefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
