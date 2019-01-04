#ifndef POOLC_AST_NODES_DECLARATION_CLASSDECLNODE_HPP_LOCK
#define POOLC_AST_NODES_DECLARATION_CLASSDECLNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "poolc/ast/collection/NodeList.hpp"
#include "poolc/ast/collection/NodeMap.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"
#include "poolc/ast/scopes/VariableScope.hpp"

class ClassDeclNode: virtual public Node {
    public:
    String & name;
    FullQualifiedName & fullQualifiedName;
    String & globalPrefix;
    String & localPrefix;
    InstanceScope * instanceScope;
    
    NodeList<TypeRefNode> &extends;
    NodeList<VariableDeclNode> &variables;
    NodeList<VariableInitInstNode> &consts;
    NodeList<MethodDeclNode> &methods;
    
    ClassDeclNode(Environment &env, MemoryInfo &mi);
    virtual ~ClassDeclNode();
    
    // uses to add default constants like CLASSNAME
    virtual VariableInitInstNode &addStringConstant(String &name, String &value);
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_DECLARATION_CLASSDECLNODE_HPP_LOCK
