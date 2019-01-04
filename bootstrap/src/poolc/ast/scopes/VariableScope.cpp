#include "poolc/ast/scopes/VariableScope.hpp"

// public
VariableScope::VariableScope(Environment &env, MemoryInfo &mi, Scope & parent, VariableDeclNode & variableDecl)
        :Scope(env, mi, &parent), Object(env, mi), finalInitializer(0), pir(0),
         variableDecl(variableDecl) {
}
VariableScope::~VariableScope() {}
VariableScope * VariableScope::isVariable() { return this; }
VariableScope * VariableScope::getVariable() { return this; }
VariableDeclNode * VariableScope::getVariableDeclNode() { return &variableDecl; }
