#include "poolc/ast/TypeManager.hpp"

// public
TypeManager::TypeManager(Environment &env, MemoryInfo &mi)
        :Object(env, mi),
         allType(env.create<AllType>()),
         anyType(env.create<AnyType>()),
         cStringType(env.create<CStringType>()),
         intType(env.create<IntType>()) {}
TypeManager::~TypeManager() {
    allType.destroy();
    anyType.destroy();
    cStringType.destroy();
    intType.destroy();
}
