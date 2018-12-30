#include "poolc/ast/scopes/CStringType.hpp"

// public
CStringType::CStringType(Environment &env, MemoryInfo &mi):Type(env, mi), Object(env, mi) {}
CStringType::~CStringType() {}
CStringType * CStringType::isCString() { return this; }
