#include "poolc/ast/scopes/ClassScope.hpp"

// public
ClassScope::ClassScope(Environment &env, MemoryInfo &mi, Scope & parent, ClassDeclNode & classDecl)
        :Scope(env, mi, &parent), Type(env, mi), Object(env, mi),
         _supers(env.create<HashMap<String, ClassScope>>()),
         classDecl(classDecl), methodsRegistered(false) {
}
ClassScope::~ClassScope() {
    _supers.destroy();
}
ClassScope * ClassScope::isClass() { return this; }
ClassDeclNode * ClassScope::getClassDeclNode() { return &classDecl; }
    
void ClassScope::addSuper(ClassScope &super) {
    _supers.set(super.getClassDeclNode()->fullQualifiedName, super);
}
bool ClassScope::hasSuper(ClassScope &super) {
    return _supers.has(super.getClassDeclNode()->fullQualifiedName);
}
