#include "poolc/ast/nodes/expression/ConstCStringExprNode.hpp"

// public
ConstCStringExprNode::ConstCStringExprNode(Environment &env, MemoryInfo &mi, String & value)
        :Object(env, mi), value(value) {}
ConstCStringExprNode::~ConstCStringExprNode() {
    value.destroy();
}
ConstCStringExprNode * ConstCStringExprNode::isConstCString() { return this; }

bool ConstCStringExprNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void ConstCStringExprNode::printDebugName(OStream & stream) {
    stream << "const-<";
    value.escapeToStream(stream);
    stream << ">";
}
