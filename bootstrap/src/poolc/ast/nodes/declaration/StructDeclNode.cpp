#include "poolc/ast/nodes/declaration/StructDeclNode.hpp"

#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"
#include "poolc/ast/nodes/instruction/VariableInitInstNode.hpp"


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

bool StructDeclNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void StructDeclNode::printDebugName(OStream & stream) {
    stream << "struct-decl-<" << fullQualifiedName << ">";
}
