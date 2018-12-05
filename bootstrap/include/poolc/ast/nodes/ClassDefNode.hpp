#ifndef POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
#include "poolc/ast/nodes/VariableDefNode.hpp"
#include "sys/collection/LinkedList.hpp"

class ClassDefNode: public Node {
    public:
    String & name;
    String & fullQualifiedName;
    String & inlinePasm;
    MutableCollection<ClassDefNode> &supers;
    MutableCollection<VariableDefNode> &variables;
    MutableCollection<MethodDefNode> &methods;
    
    ClassDefNode(Environment &env, MemoryInfo &mi)
            :Object(env, mi),
             name(env.create<String>()),
             fullQualifiedName(env.create<String>()),
             inlinePasm(env.create<String>()),
             supers(env.create<LinkedList<ClassDefNode>>()),
             variables(env.create<LinkedList<VariableDefNode>>()),
             methods(env.create<LinkedList<MethodDefNode>>()) {
    }
    virtual ~ClassDefNode() {
        name.destroy();
        fullQualifiedName.destroy();
        inlinePasm.destroy();
        {
            // factory owns all classes 
            supers.destroy();
        }
        {
            Iterator<VariableDefNode> &it = variables.iterator();
            while (it.hasNext()) { it.next().destroy(); }
            it.destroy();
            variables.destroy();
        }
        {
            Iterator<MethodDefNode> &it = methods.iterator();
            while (it.hasNext()) { it.next().destroy(); }
            it.destroy();
            methods.destroy();
        }
    }
    
    virtual bool accept(Visitor & visitor) {
        return visitor.visit(*this);
    }
};

#endif //POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK

