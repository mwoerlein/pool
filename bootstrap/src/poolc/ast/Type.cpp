#include "poolc/ast/Type.hpp"

#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"
#include "poolc/ast/scopes/IntType.hpp"
#include "poolc/ast/scopes/CStringType.hpp"

// public
Type::Type(Environment &env, MemoryInfo &mi):Object(env, mi) {}
Type::~Type() {}

ClassScope * Type::isClass() { return 0; }
InstanceScope * Type::isInstance() { return 0; }
IntType * Type::isInt() { return 0; }
CStringType * Type::isCString() { return 0; }

OStream & Type::operator >>(OStream & stream) {
    if (isInt()) {
        stream << "type-<int>";
    } else if (isCString()) {
        stream << "type-<cstring>";
    } else if (InstanceScope *scope = isInstance()) {
        stream << "type-<instance "<<scope->getClassDeclNode()->fullQualifiedName << ">";
    } else if (ClassScope *scope = isClass()) {
        stream << "type-<class "<<scope->getClassDeclNode()->fullQualifiedName << ">";
    } else {
        stream << "type-<UNKNOWN>";
    }
    return stream;
}

