#ifndef POOLC_AST_NODES_METHODDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_METHODDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "sys/String.hpp"
#include "poolc/ast/collection/NodeList.hpp"
#include "poolc/ast/nodes/InstructionNode.hpp"

enum method_kind { abstract, naked, bootstrap, normal };

class MethodDefNode: public Node {
    public:
    String & name;
    enum method_kind kind;
    scope_t scope;
    NodeList<InstructionNode> &body;
    ClassDefNode *parent;
    
    MethodDefNode(Environment &env, MemoryInfo &mi);
    virtual ~MethodDefNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_METHODDEFNODE_HPP_LOCK
