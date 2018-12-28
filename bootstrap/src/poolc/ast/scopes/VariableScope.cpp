#include "poolc/ast/scopes/VariableScope.hpp"

// public
VariableScope::VariableScope(Environment &env, MemoryInfo &mi, Scope & parent, VariableDeclNode & variableDecl)
        :Scope(env, mi, &parent), Object(env, mi),
         variableDecl(variableDecl) {
}
VariableScope::~VariableScope() {}
VariableScope * VariableScope::isVariable() { return this; }
VariableDeclNode * VariableScope::getVariableDeclNode() { return &variableDecl; }
