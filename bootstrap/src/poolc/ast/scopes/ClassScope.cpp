#include "poolc/ast/scopes/ClassScope.hpp"

// public
ClassScope::ClassScope(Environment &env, MemoryInfo &mi, Scope & parent, ClassDeclNode & classDecl)
        :Scope(env, mi, &parent), Object(env, mi),
         _supers(env.create<HashMap<String, ClassScope>>()),
         classDecl(classDecl) {
}
ClassScope::~ClassScope() {
    _supers.destroy();
}
ClassScope * ClassScope::isClass() { return this; }
ClassDeclNode * ClassScope::getClassDeclNode() { return &classDecl; }
    
void ClassScope::addSuper(ClassScope &super) {
    _supers.set(super.getClassDeclNode()->fullQualifiedName, super);
}
