#ifndef POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"
#include "poolc/ast/nodes/ClassRefNode.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
#include "poolc/ast/nodes/MethodRefNode.hpp"
#include "poolc/ast/nodes/VariableDefNode.hpp"
#include "poolc/ast/nodes/CStringConstDefNode.hpp"
#include "sys/collection/LinkedList.hpp"
#include "sys/collection/HashMap.hpp"

class ClassDefNode: public Node {
    public:
    String & name;
    String & fullQualifiedName;
    
    String & inlinePasm;
    MutableCollection<ClassRefNode> &extends;
    MutableCollection<VariableDefNode> &variables;
    MutableCollection<CStringConstDefNode> &consts;
    MutableCollection<MethodDefNode> &methods;
    
    MutableMap<String, ClassDefNode> &supers;
    MutableMap<String, MethodRefNode> &methodRefs;
    
    ClassDefNode(Environment &env, MemoryInfo &mi)
            :Object(env, mi),
             name(env.create<String>()),
             fullQualifiedName(env.create<String>()),
             inlinePasm(env.create<String>()),
             extends(env.create<LinkedList<ClassRefNode>>()),
             variables(env.create<LinkedList<VariableDefNode>>()),
             consts(env.create<LinkedList<CStringConstDefNode>>()),
             methods(env.create<LinkedList<MethodDefNode>>()),
             supers(env.create<HashMap<String, ClassDefNode>>()),
             methodRefs(env.create<HashMap<String, MethodRefNode>>()) {
    }
    virtual ~ClassDefNode() {
        name.destroy();
        fullQualifiedName.destroy();
        inlinePasm.destroy();
        
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
    
    virtual bool accept(Visitor & visitor) {
        return visitor.visit(*this);
    }
};

#endif //POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK

