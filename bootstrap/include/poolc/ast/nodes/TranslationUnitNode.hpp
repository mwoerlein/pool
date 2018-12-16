#ifndef POOLC_AST_NODES_TRANSLATIONUNITNODE_HPP_LOCK
#define POOLC_AST_NODES_TRANSLATIONUNITNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "sys/String.hpp"
#include "poolc/ast/collection/NodeList.hpp"

class TranslationUnitNode: public Node {
    public:
    String & name;
    
    NamespaceDefNode * ns;
    NodeList<UseStatementNode> &uses;
    NodeList<ClassDefNode> &classes;
    
    TranslationUnitNode(Environment &env, MemoryInfo &mi);
    virtual ~TranslationUnitNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_TRANSLATIONUNITNODE_HPP_LOCK
