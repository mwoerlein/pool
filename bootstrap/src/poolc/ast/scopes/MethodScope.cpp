#include "poolc/ast/scopes/MethodScope.hpp"

// public
MethodScope::MethodScope(Environment &env, MemoryInfo &mi, Scope & parent, MethodDeclNode & methodDecl)
        :Scope(env, mi, &parent), Object(env, mi),
         index(-1), pir(0),
         methodDecl(methodDecl) {
}
MethodScope::~MethodScope() {}
MethodScope * MethodScope::isMethod() { return this; }
MethodScope * MethodScope::getMethod() { return this; }
MethodDeclNode * MethodScope::getMethodDeclNode() { return &methodDecl; }
