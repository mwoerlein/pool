#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"

#include "poolc/ast/nodes/reference/ClassRefNode.hpp"
#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"
#include "poolc/ast/nodes/reference/MethodRefNode.hpp"
#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"
#include "poolc/ast/nodes/instruction/CStringConstAssignNode.hpp"
#include "poolc/ast/nodes/instruction/IntConstAssignNode.hpp"

#include "sys/collection/LinkedList.hpp"
#include "sys/collection/HashMap.hpp"

// public
ClassDeclNode::ClassDeclNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi),
         name(env.create<String, const char *>("UNKNOWN")),
         fullQualifiedName(env.create<FullQualifiedName>()),
         globalPrefix(env.create<String>()),
         localPrefix(env.create<String>()),
         unit(0),
         extends(env.create<NodeList<TypeRefNode>>()),
         variables(env.create<NodeList<VariableDeclNode>>()),
         consts(env.create<NodeList<CStringConstAssignNode>>()),
         intConsts(env.create<NodeList<IntConstAssignNode>>()),
         methods(env.create<NodeList<MethodDeclNode>>()),
         supers(env.create<NodeMap<ClassDeclNode>>()),
         methodRefs(env.create<NodeMap<MethodRefNode>>()) {
}
ClassDeclNode::~ClassDeclNode() {
    name.destroy();
    fullQualifiedName.destroy();
    globalPrefix.destroy();
    localPrefix.destroy();
    
    extends.destroyAll();
    variables.destroyAll();
    consts.destroyAll();
    intConsts.destroyAll();
    methods.destroyAll();
    
    // factory owns all classes 
    supers.destroy();
    methodRefs.destroyAll();
}

bool ClassDeclNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void ClassDeclNode::printDebugName(OStream & stream) {
    stream << "class-decl-<" << name << ">";
}
