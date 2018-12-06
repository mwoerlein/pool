#ifndef POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "sys/String.hpp"
#include "sys/collection/MutableCollection.hpp"
#include "sys/collection/MutableMap.hpp"

class ClassDefNode: public Node {
    public:
    String & name;
    String & fullQualifiedName;
    
    MutableCollection<ClassRefNode> &extends;
    MutableCollection<VariableDefNode> &variables;
    MutableCollection<CStringConstDefNode> &consts;
    MutableCollection<MethodDefNode> &methods;
    
    MutableMap<String, ClassDefNode> &supers;
    MutableMap<String, MethodRefNode> &methodRefs;
    
    ClassDefNode(Environment &env, MemoryInfo &mi);
    virtual ~ClassDefNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK
