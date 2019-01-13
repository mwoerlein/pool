#include "poolc/ast/nodes/instruction/WhileInstNode.hpp"

// public
WhileInstNode::WhileInstNode(Environment &env, MemoryInfo &mi, ExpressionNode &condition, BlockInstNode &block)
        :Object(env, mi), condition(condition), block(block) {}
WhileInstNode::~WhileInstNode() {
    condition.destroy();
    block.destroy();
}

bool WhileInstNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void WhileInstNode::printDebugName(OStream & stream) {
    stream << "while-<" << condition << ">";
}
