#include "poolc/ast/Scope.hpp"

#include "poolc/ast/nodes/TranslationUnitNode.hpp"
#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"
#include "poolc/ast/nodes/declaration/StructDeclNode.hpp"
#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"
#include "poolc/ast/nodes/instruction/BlockInstNode.hpp"
#include "poolc/ast/nodes/expression/MethodCallExprNode.hpp"

#include "poolc/ast/scopes/UnitScope.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"
#include "poolc/ast/scopes/StructScope.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"
#include "poolc/ast/scopes/BlockScope.hpp"
#include "poolc/ast/scopes/VariableScope.hpp"

// public
Scope::Scope(Environment &env, MemoryInfo &mi, Scope *parent)
        :Object(env, mi),
         _classes(env.create<HashMap<String, ClassScope>>()),
         _structs(env.create<HashMap<String, StructScope>>()),
         _methods(env.create<HashMap<String, MethodScope>>()),
         _variables(env.create<HashMap<String, VariableScope>>()),
         parent(parent) {
}
Scope::~Scope() {
    _classes.destroy();
    _structs.destroy();
    _methods.destroy();
    _variables.destroy();
}

UnitScope * Scope::isUnit() { return 0; }
StructScope * Scope::isStruct() { return 0; }
ClassScope * Scope::isClass() { return 0; }
InstanceScope * Scope::isInstance() { return 0; }
MethodScope * Scope::isMethod() { return 0; }
BlockScope * Scope::isBlock() { return 0; }
VariableScope * Scope::isVariable() { return 0; }

UnitScope * Scope::getUnit() { return parent ? parent->getUnit() : 0; }
StructScope * Scope::getStruct() { return parent ? parent->getStruct() : 0; }
ClassScope * Scope::getClass() { return parent ? parent->getClass() : 0; }
InstanceScope * Scope::getInstance() { return parent ? parent->getInstance() : 0; }
MethodScope * Scope::getMethod() { return parent ? parent->getMethod() : 0; }
BlockScope * Scope::getBlock() { return parent ? parent->getBlock() : 0; }
VariableScope * Scope::getVariable() { return parent ? parent->getVariable() : 0; }

TranslationUnitNode * Scope::getUnitNode() { return parent ? parent->getUnitNode() : 0; }
StructDeclNode * Scope::getStructDeclNode() { return parent ? parent->getStructDeclNode() : 0; }
ClassDeclNode * Scope::getClassDeclNode() { return parent ? parent->getClassDeclNode() : 0; }
MethodDeclNode * Scope::getMethodDeclNode() { return parent ? parent->getMethodDeclNode() : 0; }
BlockInstNode * Scope::getBlockInstNode() { return parent ? parent->getBlockInstNode() : 0; }
VariableDeclNode * Scope::getVariableDeclNode() { return parent ? parent->getVariableDeclNode() : 0; }


ClassScope * Scope::registerClass(ClassDeclNode & classDecl, String & alias) {
    ClassScope &scope = env().create<ClassScope, Scope &, ClassDeclNode &>(*this, classDecl);
    ClassScope *old = &_classes.set(alias, scope);
    if (old && (old->parent == this)) { old->destroy(); }
    return &scope;
}

StructScope * Scope::registerStruct(StructDeclNode & structDecl, String & alias) {
    StructScope &scope = env().create<StructScope, Scope &, StructDeclNode &>(*this, structDecl);
    StructScope *old = &_structs.set(alias, scope);
    if (old && (old->parent == this)) { old->destroy(); }
    return &scope;
}

NamedType * Scope::registerNamedType(NamedType & type, String & alias) {
    if (ClassScope *classScope = type.isClass()) {
        ClassScope *old = &_classes.set(alias, *classScope);
        if (old && (old->parent == this)) { old->destroy(); }
        return classScope;
    }  
    if (StructScope *structScope = type.isStruct()) {
        StructScope *old = &_structs.set(alias, *structScope);
        if (old && (old->parent == this)) { old->destroy(); }
        return structScope;
    }
}
NamedType * Scope::getNamedType(String & name) {
    if (ClassScope *scope = &_classes.get(name)) {
        return scope;
    }
    if (StructScope *scope = &_structs.get(name)) {
        return scope;
    }
    if (parent) {
        return parent->getNamedType(name);
    }
    return 0;
}

MethodScope * Scope::registerMethod(MethodDeclNode & methodDecl) {
    MethodScope &scope = env().create<MethodScope, Scope &, MethodDeclNode &>(*this, methodDecl);
    // TODO #7: generate method id from name and parameter types
    MethodScope *old = &_methods.set(methodDecl.name, scope);
    if (old && (old->parent == this)) { old->destroy(); }
    return &scope;
}
MethodScope * Scope::registerMethod(MethodScope & scope) {
    // TODO #7: generate method id from name and parameter types
    MethodScope *old = &_methods.set(scope.getMethodDeclNode()->name, scope);
    if (old && (old->parent == this)) { old->destroy(); }
    return &scope;
}
MethodScope * Scope::getMethod(String & name/*, MutableCollection<TypeRefNode> & parameters*/) {
    // TODO #7: generate method id from name and parameter types
    if (MethodScope *scope = &_methods.get(name)) {
        return scope;
    }
    if (parent) {
        return parent->getMethod(name);
    }
    return 0;
}
MethodScope * Scope::getMethod(MethodScope & scope) {
    // TODO #7: generate method id from name and parameter types
    String &name = scope.getMethodDeclNode()->name;
    if (MethodScope *scope = &_methods.get(name)) {
        return scope;
    }
    if (parent) {
        return parent->getMethod(name);
    }
    return 0;
}
MethodScope * Scope::getMethod(MethodCallExprNode & methodCall) {
    // TODO #7: generate method id from name and parameter types
    String &name = methodCall.name;
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
    if (old && (old->parent == this)) { old->destroy(); }
    return &scope;
}
VariableScope * Scope::registerVariable(VariableScope & scope) {
    VariableScope *old = &_variables.set(scope.getVariableDeclNode()->name, scope);
    if (old && (old->parent == this)) { old->destroy(); }
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

OStream & Scope::operator >>(OStream & stream) {
    printDebugName(stream);
    return stream;
}
