#include "poolc/ast/scopes/NamedType.hpp"

// public
NamedType::NamedType(Environment &env, MemoryInfo &mi):Type(env, mi), Object(env, mi) {}
NamedType::~NamedType() {}
