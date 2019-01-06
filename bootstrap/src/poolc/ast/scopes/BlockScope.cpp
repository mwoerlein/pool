#include "poolc/ast/scopes/BlockScope.hpp"

// public
BlockScope::BlockScope(Environment &env, MemoryInfo &mi, Scope & parent, BlockInstNode & block)
        :Scope(env, mi, &parent), Object(env, mi),
         block(block) {
}
BlockScope::~BlockScope() {}
BlockScope * BlockScope::isBlock() { return this; }
BlockScope * BlockScope::getBlock() { return this; }
BlockInstNode * BlockScope::getBlockInstNode() { return &block; }

void BlockScope::printDebugName(OStream & stream) {
    stream << "BlockScope of " << block;
}
