#include "poolc/ast/scopes/AnyType.hpp"

// public
AnyType::AnyType(Environment &env, MemoryInfo &mi):Type(env, mi), Object(env, mi) {}
AnyType::~AnyType() {}
AnyType * AnyType::isAny() { return this; }
