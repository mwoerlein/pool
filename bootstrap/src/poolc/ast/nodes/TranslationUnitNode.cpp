#include "poolc/ast/nodes/TranslationUnitNode.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/declaration/NamespaceDeclNode.hpp"
#include "poolc/ast/nodes/reference/UseStatementNode.hpp"

// public
TranslationUnitNode::TranslationUnitNode(Environment &env, MemoryInfo &mi, StorageElement &element)
        :HashMap(env, mi), Object(env, mi),
         element(element),
         name(env.create<String, const char *>("UNKNOWN")),
         uses(env.create<NodeList<UseStatementNode>>()),
         classes(env.create<NodeList<ClassDeclNode>>()),
         ns(0) {
}
TranslationUnitNode::~TranslationUnitNode() {
    element.destroy();
    name.destroy();
    uses.destroyAll();
    classes.destroyAll();
    if (ns) { ns->destroy(); }
}

bool TranslationUnitNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

ClassDeclNode * TranslationUnitNode::getClass(String & alias) {
    return &HashMap::get(alias);
}

void TranslationUnitNode::registerClass(String & alias, ClassDeclNode & classDef) {
    HashMap::set(alias, classDef);
}

void TranslationUnitNode::printDebugName(OStream & stream) {
    stream << "unit-<" << name << ">";
}
