#include "poolc/ast/scopes/ClassScope.hpp"

// public
ClassScope::ClassScope(Environment &env, MemoryInfo &mi, Scope & parent, ClassDeclNode & classDecl)
        :Scope(env, mi, &parent), NamedType(env, mi), Object(env, mi),
         _supers(env.create<HashMap<String, ClassScope>>()),
         _globalRequired(env.create<HashMap<String, ClassScope>>()),
         _strings(env.create<HashMap<String, String>>()),
         classDecl(classDecl), methodsRegistered(false), typesResolved(false) {
}
ClassScope::~ClassScope() {
    _globalRequired.destroy();
    _supers.destroy();
}
ClassScope * ClassScope::isClass() { return this; }
ClassScope * ClassScope::getClass() { return this; }
InstanceScope * ClassScope::getInstance() { return classDecl.instanceScope; }
ClassDeclNode * ClassScope::getClassDeclNode() { return &classDecl; }
    
void ClassScope::addSuper(ClassScope &super) {
    _supers.set(super.getClassDeclNode()->fullQualifiedName, super);
}
bool ClassScope::hasSuper(ClassScope &super) {
    return _supers.has(super.getClassDeclNode()->fullQualifiedName);
}

void ClassScope::addGlobalRequired(ClassScope &required) {
    _globalRequired.set(required.getClassDeclNode()->fullQualifiedName, required);
}

String & ClassScope::stringId(String &string, String *id) {
    if (String *has = &_strings.get(string)) {
        return *has;
    }
    if (!id) {
        id = &env().create<String>();
        *id << "string_" << _strings.size();
    }
    _strings.set(string, *id);
    return *id;
}
Iterator<String> & ClassScope::strings() {
    return _strings.keys();
}

OStream & ClassScope::operator >>(OStream & stream) {
    printDebugName(stream);
    return stream;
}

void ClassScope::printDebugName(OStream & stream) {
    stream << "ClassScope of " << classDecl;
}
