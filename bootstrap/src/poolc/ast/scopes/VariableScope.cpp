#include "poolc/ast/scopes/VariableScope.hpp"

#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"
#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"

// public
VariableScope::VariableScope(Environment &env, MemoryInfo &mi, Scope & parent, VariableDeclNode & variableDecl)
        :Scope(env, mi, &parent), Object(env, mi), finalInitializer(0), pir(0),
         variableDecl(variableDecl) {
}
VariableScope::~VariableScope() {}
VariableScope * VariableScope::isVariable() { return this; }
VariableScope * VariableScope::getVariable() { return this; }
VariableDeclNode * VariableScope::getVariableDeclNode() { return &variableDecl; }

void VariableScope::printDebugName(OStream & stream) {
    stream << "VariableScope of " << variableDecl;
    if (MethodDeclNode *node = getMethodDeclNode()) { stream << " in " << *node; }
    else if (ClassDeclNode *node = getClassDeclNode()) { stream << " in " << *node; }
}
