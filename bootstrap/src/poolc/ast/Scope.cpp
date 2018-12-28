#include "poolc/ast/Scope.hpp"

#include "poolc/ast/nodes/TranslationUnitNode.hpp"
#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"
#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"
#include "poolc/ast/nodes/instruction/BlockInstNode.hpp"

#include "poolc/ast/scopes/UnitScope.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"
#include "poolc/ast/scopes/BlockScope.hpp"
#include "poolc/ast/scopes/VariableScope.hpp"

// public
Scope::Scope(Environment &env, MemoryInfo &mi, Scope *parent)
        :Object(env, mi),
         _classes(env.create<HashMap<String, ClassScope>>()),
         _methods(env.create<HashMap<String, MethodScope>>()),
         _variables(env.create<HashMap<String, VariableScope>>()),
         parent(parent) {
}
Scope::~Scope() {
    _classes.destroy();
    _methods.destroy();
    _variables.destroy();
}

UnitScope * Scope::isUnit() { return 0; }
ClassScope * Scope::isClass() { return 0; }
InstanceScope * Scope::isInstance() { return 0; }
MethodScope * Scope::isMethod() { return 0; }
BlockScope * Scope::isBlock() { return 0; }
VariableScope * Scope::isVariable() { return 0; }

TranslationUnitNode * Scope::getUnitNode() { return parent ? parent->getUnitNode() : 0; }
ClassDeclNode * Scope::getClassDeclNode() { return parent ? parent->getClassDeclNode() : 0; }
MethodDeclNode * Scope::getMethodDeclNode() { return parent ? parent->getMethodDeclNode() : 0; }
BlockInstNode * Scope::getBlockInstNode() { return parent ? parent->getBlockInstNode() : 0; }
VariableDeclNode * Scope::getVariableDeclNode() { return parent ? parent->getVariableDeclNode() : 0; }


ClassScope * Scope::registerClass(ClassDeclNode & classDecl, String & alias) {
    ClassScope &scope = env().create<ClassScope, Scope &, ClassDeclNode &>(*this, classDecl);
    ClassScope *old = &_classes.set(alias, scope);
    if (old) { old->destroy(); }
    return &scope;
}
ClassScope * Scope::registerClass(ClassScope & scope) {
    ClassScope *old = &_classes.set(scope.getClassDeclNode()->name, scope);
    if (old) { old->destroy(); }
    return &scope;
}
ClassScope * Scope::registerClass(ClassScope & scope, String & alias) {
    ClassScope *old = &_classes.set(alias, scope);
    if (old) { old->destroy(); }
    return &scope;
}
ClassScope * Scope::getClass(String & name) {
    if (ClassScope *scope = &_classes.get(name)) {
        return scope;
    }
    if (parent) {
        return parent->getClass(name);
    }
    return 0;
}

MethodScope * Scope::registerMethod(MethodDeclNode & methodDecl) {
    MethodScope &scope = env().create<MethodScope, Scope &, MethodDeclNode &>(*this, methodDecl);
    // TODO: #7 generate method id from name and parameter types
    MethodScope *old = &_methods.set(methodDecl.name, scope);
    if (old) { old->destroy(); }
    return &scope;
}
MethodScope * Scope::getMethod(String & name/*, MutableCollection<TypeRefNode> & parameters*/) {
    // TODO: #7 generate method id from name and parameter types
    if (MethodScope *scope = &_methods.get(name)) {
        return scope;
    }
    if (parent) {
        return parent->getMethod(name);
    }
    return 0;
}
MethodScope * Scope::getMethod(MethodScope & scope) {
    // TODO: #7 generate method id from name and parameter types
    String &name = scope.getMethodDeclNode()->name;
    if (MethodScope *scope = &_methods.get(name)) {
        return scope;
    }
    if (parent) {
        return parent->getMethod(name);
    }
    return 0;
}

VariableScope * Scope::registerVariable(VariableDeclNode & variableDecl) {
    VariableScope &scope = env().create<VariableScope, Scope &, VariableDeclNode &>(*this, variableDecl);
    VariableScope *old = &_variables.set(variableDecl.name, scope);
    if (old) { old->destroy(); }
    return &scope;
}
VariableScope * Scope::getVariable(String & name) {
    if (VariableScope *scope = &_variables.get(name)) {
        return scope;
    }
    if (parent) {
        return parent->getVariable(name);
    }
    return 0;
}
