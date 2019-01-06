#include "poolc/ast/scopes/InstanceScope.hpp"

// public
InstanceScope::InstanceScope(Environment &env, MemoryInfo &mi, Scope & parent, ClassDeclNode & classDecl)
        :Scope(env, mi, &parent), Type(env, mi), Object(env, mi),
         classDecl(classDecl) {
}
InstanceScope::~InstanceScope() {}
InstanceScope * InstanceScope::isInstance() { return this; }
InstanceScope * InstanceScope::getInstance() { return this; }
ClassDeclNode * InstanceScope::getClassDeclNode() { return &classDecl; }

OStream & InstanceScope::operator >>(OStream & stream) {
    printDebugName(stream);
    return stream;
}

void InstanceScope::printDebugName(OStream & stream) {
    stream << "InstanceScope of " << classDecl;
}
