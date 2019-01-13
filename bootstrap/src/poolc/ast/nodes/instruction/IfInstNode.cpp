#include "poolc/ast/nodes/instruction/IfInstNode.hpp"

// public
IfInstNode::IfInstNode(Environment &env, MemoryInfo &mi, ExpressionNode &condition, BlockInstNode &trueBlock, BlockInstNode &falseBlock)
        :Object(env, mi), condition(condition), trueBlock(trueBlock), falseBlock(falseBlock) {}
IfInstNode::~IfInstNode() {
    condition.destroy();
    trueBlock.destroy();
    falseBlock.destroy();
}

bool IfInstNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void IfInstNode::printDebugName(OStream & stream) {
    stream << "if-<" << condition << ">";
}
