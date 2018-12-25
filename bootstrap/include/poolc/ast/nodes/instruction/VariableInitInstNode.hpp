#ifndef POOLC_AST_NODES_INSTRUCTION_VARIABLEINITINSTNODE_HPP_LOCK
#define POOLC_AST_NODES_INSTRUCTION_VARIABLEINITINSTNODE_HPP_LOCK

#include "poolc/ast/nodes/InstructionNode.hpp"

#include "poolc/ast/collection/NodeList.hpp"
#include "poolc/ast/nodes/ExpressionNode.hpp"
#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"
#include "poolc/ast/nodes/expression/MethodCallExprNode.hpp"

class VariableInitInstNode: public InstructionNode {
    public:
    NodeList<VariableDeclNode> &variables;
    ExpressionNode & initializer;
    scope_t scope;
    bool final;
    bool global;
    
    VariableInitInstNode(Environment &env, MemoryInfo &mi, VariableDeclNode & var, ExpressionNode & initializer);
    VariableInitInstNode(Environment &env, MemoryInfo &mi, MutableCollection<VariableDeclNode> & vars, MethodCallExprNode & initializer);
    virtual ~VariableInitInstNode();
    
    virtual bool accept(Visitor & visitor) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_NODES_INSTRUCTION_VARIABLEINITINSTNODE_HPP_LOCK
