#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"
#include "poolc/ast/nodes/instruction/BlockInstNode.hpp"
#include "poolc/ast/nodes/reference/TypeRefNode.hpp"

// public
MethodDeclNode::MethodDeclNode(Environment &env, MemoryInfo &mi, String &name, MutableCollection<TypeRefNode> &rets, MutableCollection<VariableDeclNode> &params)
        :Object(env, mi),
         name(name),
         body(env.create<BlockInstNode>()),
         returnTypes(env.create<NodeList<TypeRefNode>>()),
         parameters(env.create<NodeList<VariableDeclNode>>()),
         kind(normal), scope(scope_instance),
         parent(0), index(-1) {
    returnTypes.addAll(rets);
    parameters.addAll(params);
}
MethodDeclNode::MethodDeclNode(Environment &env, MemoryInfo &mi, String &name, BlockInstNode &body, MutableCollection<TypeRefNode> &rets, MutableCollection<VariableDeclNode> &params)
        :Object(env, mi),
         name(name),
         body(body),
         returnTypes(env.create<NodeList<TypeRefNode>>()),
         parameters(env.create<NodeList<VariableDeclNode>>()),
         kind(normal), scope(scope_instance),
         parent(0), index(-1) {
    returnTypes.addAll(rets);
    parameters.addAll(params);
}
MethodDeclNode::~MethodDeclNode() {
    name.destroy();
    body.destroy();
    returnTypes.destroyAll();
    parameters.destroyAll();
}

bool MethodDeclNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void MethodDeclNode::printDebugName(OStream & stream) {
    stream << "method-decl-<" << name << ">";
}
