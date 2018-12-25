#include "poolc/ast/Visitor.hpp"

// public
Visitor::~Visitor() {}

//bool Visitor::visit(TranslationUnitNode & translationUnit) { return false; }

bool Visitor::visit(ClassDeclNode & classDef) { return false; }
bool Visitor::visit(MethodDeclNode & methodDef) { return false; }
bool Visitor::visit(NamespaceDeclNode & namespaceDef) { return false; }
bool Visitor::visit(VariableDeclNode & variableDef) { return false; }

bool Visitor::visit(ClassRefNode & classRef) { return false; }
bool Visitor::visit(CStringRefNode & type) { return false; }
bool Visitor::visit(IntRefNode & type) { return false; }
bool Visitor::visit(MethodRefNode & methodRef) { return false; }
bool Visitor::visit(UseStatementNode & useStmt) { return false; }

bool Visitor::visit(BlockInstNode & block) { return false; }
bool Visitor::visit(InlinePasmInstNode & pasmInstruction) { return false; }
bool Visitor::visit(ReturnInstNode & returnInst) { return false; }
bool Visitor::visit(VariableInitInstNode & variableInit) { return false; }

bool Visitor::visit(AssignmentExprNode & assignment) { return false; }
bool Visitor::visit(ConstCStringExprNode & constCString) { return false; }
bool Visitor::visit(ConstIntExprNode & constInt) { return false; }
bool Visitor::visit(MethodCallExprNode & methodCall) { return false; }
bool Visitor::visit(ThisExprNode & constThis) { return false; }
bool Visitor::visit(VariableExprNode & variable) { return false; }
