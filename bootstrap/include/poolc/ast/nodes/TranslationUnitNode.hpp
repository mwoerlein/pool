#ifndef POOLC_AST_NODES_TRANSLATIONUNITNODE_HPP_LOCK
#define POOLC_AST_NODES_TRANSLATIONUNITNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"
#include "sys/collection/HashMap.hpp"

#include "sys/String.hpp"
#include "poolc/ast/collection/NodeList.hpp"
#include "poolc/storage/StorageElement.hpp"

class TranslationUnitNode: public Node, private HashMap<String, ClassDefNode> {
    public:
    StorageElement &element;
    String &name;
    
    NamespaceDefNode * ns;
    NodeList<UseStatementNode> &uses;
    NodeList<ClassDefNode> &classes;
    
    TranslationUnitNode(Environment &env, MemoryInfo &mi, StorageElement &element);
    virtual ~TranslationUnitNode();
    
    virtual bool accept(Visitor & visitor) override;
    
    virtual ClassDefNode * getClass(String & alias);
    virtual void registerClass(String & alias, ClassDefNode & classDef);
};

#endif //POOLC_AST_NODES_TRANSLATIONUNITNODE_HPP_LOCK
