#ifndef POOLC_AST_NODES_DECLARATION_METHODDECLNODE_HPP_LOCK
#define POOLC_AST_NODES_DECLARATION_METHODDECLNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "poolc/ast/Type.hpp"
#include "poolc/ast/collection/NodeList.hpp"
#include "poolc/ast/nodes/InstructionNode.hpp"

enum method_kind { abstract, naked, normal, entry };

class MethodDeclNode: virtual public Node {
    public:
    String & name;
    BlockInstNode &body;
    NodeList<TypeRefNode> &returnTypes;
    NodeList<VariableDeclNode> &parameters;
    
    LinkedList<Type> &resolvedReturns;
    
    enum method_kind kind;
    bool global;
    int index;
    
    MethodDeclNode(Environment &env, MemoryInfo &mi, String &name, MutableCollection<TypeRefNode> &rets, MutableCollection<VariableDeclNode> &params);
    MethodDeclNode(Environment &env, MemoryInfo &mi, String &name, BlockInstNode &body, MutableCollection<TypeRefNode> &rets, MutableCollection<VariableDeclNode> &params);
    virtual ~MethodDeclNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_DECLARATION_METHODDECLNODE_HPP_LOCK
