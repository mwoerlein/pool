#include "poolc/ast/nodes/instruction/VariableInitInstNode.hpp"

// public
VariableInitInstNode::VariableInitInstNode(Environment &env, MemoryInfo &mi, VariableDeclNode & var, ExpressionNode & initializer)
        :Object(env, mi),
         variables(env.create<NodeList<VariableDeclNode>>()),
         initializer(initializer), scope(scope_block), final(false) {
    variables.add(var);
}
VariableInitInstNode::VariableInitInstNode(Environment &env, MemoryInfo &mi, MutableCollection<VariableDeclNode> & vars, MethodCallExprNode & initializer)
        :Object(env, mi),
         variables(env.create<NodeList<VariableDeclNode>>()),
         initializer(initializer), scope(scope_block), final(false) {
    variables.addAll(vars);
}
VariableInitInstNode::~VariableInitInstNode() {
    variables.destroyAll();
    initializer.destroy();
}

bool VariableInitInstNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void VariableInitInstNode::printDebugName(OStream & stream) {
    stream << "variable-init-<";
    {
        Iterator<VariableDeclNode> &it = variables.iterator();
        if (it.hasNext()) {
            stream << it.next().name;
        }
        while (it.hasNext()) {
            stream << "," << it.next().name;
        }
        it.destroy();
    }
    stream << ">";
}
