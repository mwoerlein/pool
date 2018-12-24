#ifndef POOLC_AST_NODES_DECLARATION_METHODDECLNODE_HPP_LOCK
#define POOLC_AST_NODES_DECLARATION_METHODDECLNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "poolc/ast/collection/NodeList.hpp"
#include "poolc/ast/nodes/InstructionNode.hpp"

enum method_kind { abstract, naked, normal };

class MethodDeclNode: public Node {
    public:
    String & name;
    enum method_kind kind;
    scope_t scope;
    NodeList<InstructionNode> &body;
    ClassDeclNode *parent;
    int index;
    
    MethodDeclNode(Environment &env, MemoryInfo &mi);
    virtual ~MethodDeclNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_DECLARATION_METHODDECLNODE_HPP_LOCK
