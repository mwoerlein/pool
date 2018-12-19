#include "poolc/ast/nodes/ClassDefNode.hpp"

#include "poolc/ast/nodes/ClassRefNode.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
#include "poolc/ast/nodes/MethodRefNode.hpp"
#include "poolc/ast/nodes/VariableDefNode.hpp"
#include "poolc/ast/nodes/CStringConstDefNode.hpp"
#include "poolc/ast/nodes/IntConstDefNode.hpp"

#include "sys/collection/LinkedList.hpp"
#include "sys/collection/HashMap.hpp"

// public
ClassDefNode::ClassDefNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi),
         name(env.create<String>()),
         fullQualifiedName(env.create<FullQualifiedName>()),
         globalPrefix(env.create<String>()),
         localPrefix(env.create<String>()),
         unit(0),
         extends(env.create<NodeList<ClassRefNode>>()),
         variables(env.create<NodeList<VariableDefNode>>()),
         consts(env.create<NodeList<CStringConstDefNode>>()),
         intConsts(env.create<NodeList<IntConstDefNode>>()),
         methods(env.create<NodeList<MethodDefNode>>()),
         supers(env.create<NodeMap<ClassDefNode>>()),
         methodRefs(env.create<NodeMap<MethodRefNode>>()) {
}
ClassDefNode::~ClassDefNode() {
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

bool ClassDefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
