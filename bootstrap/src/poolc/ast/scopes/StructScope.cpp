#include "poolc/ast/scopes/StructScope.hpp"

// public
StructScope::StructScope(Environment &env, MemoryInfo &mi, Scope & parent, StructDeclNode & structDecl)
        :Scope(env, mi, &parent), NamedType(env, mi), Object(env, mi),
         structDecl(structDecl), instanceSize(-1), typesResolved(false) {
}
StructScope::~StructScope() {}
StructScope * StructScope::isStruct() { return this; }
StructScope * StructScope::getStruct() { return this; }
StructDeclNode * StructScope::getStructDeclNode() { return &structDecl; }
    
OStream & StructScope::operator >>(OStream & stream) {
    printDebugName(stream);
    return stream;
}

void StructScope::printDebugName(OStream & stream) {
    stream << "StructScope of " << structDecl;
}
