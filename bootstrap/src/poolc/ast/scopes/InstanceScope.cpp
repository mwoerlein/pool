#include "poolc/ast/scopes/InstanceScope.hpp"

// public
InstanceScope::InstanceScope(Environment &env, MemoryInfo &mi, ClassDeclNode & classDecl, ClassScope & parent)
        :Scope(env, mi, &parent), Object(env, mi),
         classDecl(classDecl) {
}
InstanceScope::~InstanceScope() {}
InstanceScope * InstanceScope::isInstance() { return this; }
ClassDeclNode * InstanceScope::getClassDeclNode() { return &classDecl; }
