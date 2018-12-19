#ifndef POOLC_AST_NODES_TRANSLATIONUNITNODE_HPP_LOCK
#define POOLC_AST_NODES_TRANSLATIONUNITNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"
#include "sys/collection/HashMap.hpp"

#include "sys/String.hpp"
#include "poolc/ast/collection/NodeList.hpp"

class TranslationUnitNode: public Node, private HashMap<String, ClassDefNode> {
    public:
    String & name;
    
    NamespaceDefNode * ns;
    NodeList<UseStatementNode> &uses;
    NodeList<ClassDefNode> &classes;
    
    TranslationUnitNode(Environment &env, MemoryInfo &mi);
    virtual ~TranslationUnitNode();
    
    virtual bool accept(Visitor & visitor) override;
    
    virtual ClassDefNode * getClass(String & alias);
    virtual void registerClass(String & alias, ClassDefNode & classDef);
};

#endif //POOLC_AST_NODES_TRANSLATIONUNITNODE_HPP_LOCK
