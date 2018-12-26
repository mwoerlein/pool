#include "poolc/ast/scopes/ClassScope.hpp"

// public
ClassScope::ClassScope(Environment &env, MemoryInfo &mi, ClassDeclNode & classDecl, UnitScope & parent)
        :Scope(env, mi, &parent), Object(env, mi),
         classDecl(classDecl) {
}
ClassScope::~ClassScope() {}
ClassScope * ClassScope::isClass() { return this; }
ClassDeclNode * ClassScope::getClassDeclNode() { return &classDecl; }
