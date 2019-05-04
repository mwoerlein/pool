#ifndef POOLC_AST_NODES_DECLARATION_STRUCTDECLNODE_HPP_LOCK
#define POOLC_AST_NODES_DECLARATION_STRUCTDECLNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "poolc/ast/collection/NodeList.hpp"

class StructDeclNode: virtual public Node {
    public:
    String & name;
    FullQualifiedName & fullQualifiedName;
    
    NodeList<VariableDeclNode> &variables;
    NodeList<VariableInitInstNode> &consts;
    
    StructDeclNode(Environment &env, MemoryInfo &mi);
    virtual ~StructDeclNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_DECLARATION_STRUCTDECLNODE_HPP_LOCK
