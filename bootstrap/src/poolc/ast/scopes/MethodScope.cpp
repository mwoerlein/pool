#include "poolc/ast/scopes/MethodScope.hpp"

// public
MethodScope::MethodScope(Environment &env, MemoryInfo &mi, MethodDeclNode & methodDecl, InstanceScope & parent)
        :Scope(env, mi, &parent), Object(env, mi),
         methodDecl(methodDecl) {
}
MethodScope::~MethodScope() {}
MethodScope * MethodScope::isMethod() { return this; }
MethodDeclNode * MethodScope::getMethodDeclNode() { return &methodDecl; }
