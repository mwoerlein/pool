#include "poolc/ast/Type.hpp"

#include "poolc/ast/scopes/AllType.hpp"
#include "poolc/ast/scopes/AnyType.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/CStringType.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"
#include "poolc/ast/scopes/IntType.hpp"
#include "poolc/ast/scopes/StructScope.hpp"

// public
Type::Type(Environment &env, MemoryInfo &mi):Object(env, mi) {}
Type::~Type() {}

AllType * Type::isAll() { return 0; }
AnyType * Type::isAny() { return 0; }
StructScope * Type::isStruct() { return 0; }
ClassScope * Type::isClass() { return 0; }
CStringType * Type::isCString() { return 0; }
InstanceScope * Type::isInstance() { return 0; }
IntType * Type::isInt() { return 0; }

OStream & Type::operator >>(OStream & stream) {
    if (isAll()) {
        stream << "type-<ALL>";
    } else if (isAny()) {
        stream << "type-<ANY>";
    } else if (isInt()) {
        stream << "type-<int>";
    } else if (isCString()) {
        stream << "type-<cstring>";
    } else {
        stream << "type-<UNKNOWN>";
    }
    return stream;
}

