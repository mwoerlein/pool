#include "poolc/ast/nodes/TranslationUnitNode.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/declaration/NamespaceDeclNode.hpp"
#include "poolc/ast/nodes/declaration/StructDeclNode.hpp"
#include "poolc/ast/nodes/reference/UseStatementNode.hpp"

// public
TranslationUnitNode::TranslationUnitNode(Environment &env, MemoryInfo &mi, StorageElement &element)
        :Object(env, mi),
         element(element),
         name(env.create<String, const char *>("UNKNOWN")),
         uses(env.create<NodeList<UseStatementNode>>()),
         classes(env.create<NodeList<ClassDeclNode>>()),
         structs(env.create<NodeList<StructDeclNode>>()),
         ns(0) {
}
TranslationUnitNode::~TranslationUnitNode() {
    element.destroy();
    name.destroy();
    uses.destroyAll();
    classes.destroyAll();
    structs.destroyAll();
    if (ns) { ns->destroy(); }
}

bool TranslationUnitNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void TranslationUnitNode::printDebugName(OStream & stream) {
    stream << "unit-<" << name << ">";
}
