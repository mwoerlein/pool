#include "poolc/ast/nodes/instruction/ReturnInstNode.hpp"

// public
ReturnInstNode::ReturnInstNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi), values(env.create<NodeList<ExpressionNode>>()) {}
ReturnInstNode::ReturnInstNode(Environment &env, MemoryInfo &mi, ExpressionNode &val)
        :Object(env, mi), values(env.create<NodeList<ExpressionNode>>()) {
    values.add(val);
}
ReturnInstNode::ReturnInstNode(Environment &env, MemoryInfo &mi, MutableCollection<ExpressionNode> &vals)
        :Object(env, mi), values(env.create<NodeList<ExpressionNode>>()) {
    values.addAll(vals);
}
ReturnInstNode::~ReturnInstNode() {
    values.destroyAll();
}

bool ReturnInstNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void ReturnInstNode::printDebugName(OStream & stream) {
    stream << "return";
}
