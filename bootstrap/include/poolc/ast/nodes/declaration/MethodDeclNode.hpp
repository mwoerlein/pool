#ifndef POOLC_AST_NODES_DECLARATION_METHODDECLNODE_HPP_LOCK
#define POOLC_AST_NODES_DECLARATION_METHODDECLNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "poolc/ast/collection/NodeList.hpp"
#include "poolc/ast/nodes/InstructionNode.hpp"

enum method_kind { abstract, naked, normal };

class MethodDeclNode: public Node {
    public:
    String & name;
    BlockInstNode &body;
    enum method_kind kind;
    scope_t scope;
    ClassDeclNode *parent;
    int index;
    
    MethodDeclNode(Environment &env, MemoryInfo &mi, String &name);
    MethodDeclNode(Environment &env, MemoryInfo &mi, String &name, BlockInstNode &body);
    virtual ~MethodDeclNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_DECLARATION_METHODDECLNODE_HPP_LOCK
