#include "poolc/ast/scopes/AllType.hpp"

// public
AllType::AllType(Environment &env, MemoryInfo &mi):Type(env, mi), Object(env, mi) {}
AllType::~AllType() {}
AllType * AllType::isAll() { return this; }
