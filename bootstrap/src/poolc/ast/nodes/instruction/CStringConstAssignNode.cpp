#include "poolc/ast/nodes/instruction/CStringConstAssignNode.hpp"

// public
CStringConstAssignNode::CStringConstAssignNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), name(env.create<String, const char *>("UNKNOWN")), value(env.create<String>()) {}
CStringConstAssignNode::~CStringConstAssignNode() {
    name.destroy();
    value.destroy();
}

bool CStringConstAssignNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void CStringConstAssignNode::printDebugName(OStream & stream) {
    stream << "cstring-const-<" << name << ">";
}
