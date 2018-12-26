#include "poolc/ast/Scope.hpp"

// public
Scope::Scope(Environment &env, MemoryInfo &mi, Scope *parent)
        :Object(env, mi),
         classes(env.create<HashMap<String, ClassDeclNode>>()),
         methods(env.create<HashMap<String, MethodDeclNode>>()),
         variables(env.create<HashMap<String, VariableDeclNode>>()),
         parent(parent) {
}
Scope::~Scope() {
    classes.destroy();
    methods.destroy();
    variables.destroy();
}

UnitScope * Scope::isUnit() { return 0; }
ClassScope * Scope::isClass() { return 0; }
InstanceScope * Scope::isInstance() { return 0; }
MethodScope * Scope::isMethod() { return 0; }
BlockScope * Scope::isBlock() { return 0; }

TranslationUnitNode * Scope::getUnitNode() { return parent ? parent->getUnitNode() : 0; }
ClassDeclNode * Scope::getClassDeclNode() { return parent ? parent->getClassDeclNode() : 0; }
MethodDeclNode * Scope::getMethodDeclNode() { return parent ? parent->getMethodDeclNode() : 0; }
BlockInstNode * Scope::getBlockInstNode() { return parent ? parent->getBlockInstNode() : 0; }


bool Scope::registerClass(ClassDeclNode & classDecl, String & alias) {
    classes.set(alias, classDecl);
    return true;
}
ClassDeclNode * Scope::getClass(String & name) {
    if (ClassDeclNode *decl = &classes.get(name)) {
        return decl;
    }
    if (parent) {
        return parent->getClass(name);
    }
    return 0;
}

bool Scope::registerMethod(MethodDeclNode & methodDecl) {
    // TODO: #7 generate method id from name and parameter types
    methods.set(methodDecl.name, methodDecl);
    return true;
}
MethodDeclNode * Scope::getMethod(String & name/*, MutableCollection<TypeRefNode> & parameters*/) {
    // TODO: #7 generate method id from name and parameter types
    if (MethodDeclNode *decl = &methods.get(name)) {
        return decl;
    }
    if (parent) {
        return parent->getMethod(name);
    }
    return 0;
}

bool Scope::registerVariable(VariableDeclNode & variableDecl) {
    variables.set(variableDecl.name, variableDecl);
    return true;
}
VariableDeclNode * Scope::getVariable(String & name) {
    if (VariableDeclNode *decl = &variables.get(name)) {
        return decl;
    }
    if (parent) {
        return parent->getVariable(name);
    }
    return 0;
}
