#include "poolc/ast/scopes/IntType.hpp"

// public
IntType::IntType(Environment &env, MemoryInfo &mi):Type(env, mi), Object(env, mi) {}
IntType::~IntType() {}
IntType * IntType::isInt() { return this; }
