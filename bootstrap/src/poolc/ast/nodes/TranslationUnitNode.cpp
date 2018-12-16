#include "poolc/ast/nodes/TranslationUnitNode.hpp"

#include "poolc/ast/nodes/ClassDefNode.hpp"
#include "poolc/ast/nodes/NamespaceDefNode.hpp"
#include "poolc/ast/nodes/UseStatementNode.hpp"

// public
TranslationUnitNode::TranslationUnitNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi),
         name(env.create<String>()),
         uses(env.create<NodeList<UseStatementNode>>()),
         classes(env.create<NodeList<ClassDefNode>>()),
         ns(0) {
}
TranslationUnitNode::~TranslationUnitNode() {
    name.destroy();
    uses.destroyAll();
    classes.destroyAll();
    if (ns) { ns->destroy(); }
}

bool TranslationUnitNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
