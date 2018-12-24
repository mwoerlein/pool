#ifndef POOLC_AST_NODES_DECLARATION_CLASSDECLNODE_HPP_LOCK
#define POOLC_AST_NODES_DECLARATION_CLASSDECLNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "poolc/ast/collection/NodeList.hpp"
#include "poolc/ast/collection/NodeMap.hpp"

class ClassDeclNode: public Node {
    public:
    String & name;
    FullQualifiedName & fullQualifiedName;
    String & globalPrefix;
    String & localPrefix;
    TranslationUnitNode * unit;
    
    NodeList<ClassRefNode> &extends;
    NodeList<VariableDeclNode> &variables;
    NodeList<CStringConstAssignNode> &consts;
    NodeList<IntConstAssignNode> &intConsts;
    NodeList<MethodDeclNode> &methods;
    
    NodeMap<ClassDeclNode> &supers;
    NodeMap<MethodRefNode> &methodRefs;
    
    ClassDeclNode(Environment &env, MemoryInfo &mi);
    virtual ~ClassDeclNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_DECLARATION_CLASSDECLNODE_HPP_LOCK
