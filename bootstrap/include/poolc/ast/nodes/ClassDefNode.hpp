#ifndef POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "sys/String.hpp"
#include "poolc/ast/collection/NodeList.hpp"
#include "poolc/ast/collection/NodeMap.hpp"
#include "poolc/parser/token/FullQualifiedName.hpp"

class ClassDefNode: public Node {
    public:
    String & name;
    FullQualifiedName & fullQualifiedName;
    String & globalPrefix;
    String & localPrefix;
    TranslationUnitNode * unit;
    
    NodeList<ClassRefNode> &extends;
    NodeList<VariableDefNode> &variables;
    NodeList<CStringConstDefNode> &consts;
    NodeList<IntConstDefNode> &intConsts;
    NodeList<MethodDefNode> &methods;
    
    NodeMap<ClassDefNode> &supers;
    NodeMap<MethodRefNode> &methodRefs;
    
    ClassDefNode(Environment &env, MemoryInfo &mi);
    virtual ~ClassDefNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_CLASSDEFNODE_HPP_LOCK
