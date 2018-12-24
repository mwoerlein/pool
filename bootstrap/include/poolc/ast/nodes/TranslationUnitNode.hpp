#ifndef POOLC_AST_NODES_TRANSLATIONUNITNODE_HPP_LOCK
#define POOLC_AST_NODES_TRANSLATIONUNITNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"
#include "sys/collection/HashMap.hpp"

#include "poolc/ast/collection/NodeList.hpp"
#include "poolc/storage/StorageElement.hpp"

class TranslationUnitNode: public Node, private HashMap<String, ClassDeclNode> {
    public:
    StorageElement &element;
    String &name;
    
    NamespaceDeclNode * ns;
    NodeList<UseStatementNode> &uses;
    NodeList<ClassDeclNode> &classes;
    
    TranslationUnitNode(Environment &env, MemoryInfo &mi, StorageElement &element);
    virtual ~TranslationUnitNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
    
    virtual ClassDeclNode * getClass(String & alias);
    virtual void registerClass(String & alias, ClassDeclNode & classDef);
};

#endif //POOLC_AST_NODES_TRANSLATIONUNITNODE_HPP_LOCK
