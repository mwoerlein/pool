#include "poolc/ast/scopes/MethodScope.hpp"

// public
MethodScope::MethodScope(Environment &env, MemoryInfo &mi, Scope & parent, MethodDeclNode & methodDecl)
        :Scope(env, mi, &parent), Object(env, mi),
         index(-1),
         methodDecl(methodDecl) {
}
MethodScope::~MethodScope() {}
MethodScope * MethodScope::isMethod() { return this; }
MethodDeclNode * MethodScope::getMethodDeclNode() { return &methodDecl; }
