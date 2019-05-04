#include "poolc/ast/nodes/declaration/StructDeclNode.hpp"

#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"
#include "poolc/ast/nodes/instruction/VariableInitInstNode.hpp"
#include "poolc/ast/nodes/expression/ConstIntExprNode.hpp"
#include "poolc/ast/nodes/reference/IntRefNode.hpp"

// public
StructDeclNode::StructDeclNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi),
         name(env.create<String, const char *>("UNKNOWN")),
         fullQualifiedName(env.create<FullQualifiedName>()),
         variables(env.create<NodeList<VariableDeclNode>>()),
         consts(env.create<NodeList<VariableInitInstNode>>()) {
}
StructDeclNode::~StructDeclNode() {
    name.destroy();
    fullQualifiedName.destroy();
    
    variables.destroyAll();
    consts.destroyAll();
}

VariableInitInstNode &StructDeclNode::addIntConstant(String &name, int value) {
    TypeRefNode &ref = env().create<IntRefNode>();
    VariableDeclNode &decl = env().create<VariableDeclNode, TypeRefNode&, String&>(ref, name);
    ConstIntExprNode &cint = env().create<ConstIntExprNode, int>(value);
    VariableInitInstNode &init = env().create<VariableInitInstNode, VariableDeclNode &, ExpressionNode &>(decl, cint);
    init.final = true;
    decl.global = true;
    consts.add(init);
    return init;
}

bool StructDeclNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void StructDeclNode::printDebugName(OStream & stream) {
    stream << "struct-decl-<" << fullQualifiedName << ">";
}
