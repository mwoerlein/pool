#include "poolc/ast/nodes/ClassDefNode.hpp"

#include "poolc/ast/nodes/ClassRefNode.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
#include "poolc/ast/nodes/MethodRefNode.hpp"
#include "poolc/ast/nodes/VariableDefNode.hpp"
#include "poolc/ast/nodes/CStringConstDefNode.hpp"

#include "sys/collection/LinkedList.hpp"
#include "sys/collection/HashMap.hpp"

// public
ClassDefNode::ClassDefNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi),
         name(env.create<String>()),
         fullQualifiedName(env.create<String>()),
         extends(env.create<LinkedList<ClassRefNode>>()),
         variables(env.create<LinkedList<VariableDefNode>>()),
         consts(env.create<LinkedList<CStringConstDefNode>>()),
         methods(env.create<LinkedList<MethodDefNode>>()),
         supers(env.create<HashMap<String, ClassDefNode>>()),
         methodRefs(env.create<HashMap<String, MethodRefNode>>()) {
}
ClassDefNode::~ClassDefNode() {
    name.destroy();
    fullQualifiedName.destroy();
    
    {
        Iterator<ClassRefNode> &it = extends.iterator();
        while (it.hasNext()) { it.next().destroy(); }
        it.destroy();
        extends.destroy();
    }
    {
        Iterator<VariableDefNode> &it = variables.iterator();
        while (it.hasNext()) { it.next().destroy(); }
        it.destroy();
        variables.destroy();
    }
    {
        Iterator<CStringConstDefNode> &it = consts.iterator();
        while (it.hasNext()) { it.next().destroy(); }
        it.destroy();
        consts.destroy();
    }
    {
        Iterator<MethodDefNode> &it = methods.iterator();
        while (it.hasNext()) { it.next().destroy(); }
        it.destroy();
        methods.destroy();
    }
    
    {
        // factory owns all classes 
        supers.destroy();
    }
    {
        Iterator<MethodRefNode> &it = methodRefs.values();
        while (it.hasNext()) { it.next().destroy(); }
        it.destroy();
        methodRefs.destroy();
    }
}

bool ClassDefNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}
