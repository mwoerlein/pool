#ifndef POOLC_AST_NODES_USESTATEMENTNODE_HPP_LOCK
#define POOLC_AST_NODES_USESTATEMENTNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "poolc/parser/token/FullQualifiedName.hpp"

class UseStatementNode: public Node {
    public:
    String & alias;
    FullQualifiedName & name;
    
    UseStatementNode(Environment &env, MemoryInfo &mi);
    virtual ~UseStatementNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_USESTATEMENTNODE_HPP_LOCK
