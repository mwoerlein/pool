#include "poolc/ast/scopes/BlockScope.hpp"

// public
BlockScope::BlockScope(Environment &env, MemoryInfo &mi, BlockInstNode & block, MethodScope & parent)
        :Scope(env, mi, &parent), Object(env, mi),
         block(block) {
}
BlockScope::~BlockScope() {}
BlockScope * BlockScope::isBlock() { return this; }
BlockInstNode * BlockScope::getBlockInstNode() { return &block; }
