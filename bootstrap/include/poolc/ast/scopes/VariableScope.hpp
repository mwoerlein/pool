#ifndef POOLC_AST_SCOPES_VARIABLESCOPE_HPP_LOCK
#define POOLC_AST_SCOPES_VARIABLESCOPE_HPP_LOCK

#include "poolc/ast/Scope.hpp"

#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"

class PIRLocation;
class VariableScope: public Scope {
    private:
    VariableDeclNode & variableDecl;
    
    public:
    PIRLocation *pir;
    
    VariableScope(Environment &env, MemoryInfo &mi, Scope & parent, VariableDeclNode & variableDecl);
    virtual ~VariableScope();
    virtual VariableScope * isVariable() override;
    virtual VariableScope * getVariable() override;
    virtual VariableDeclNode * getVariableDeclNode() override;
};

#endif //POOLC_AST_SCOPES_VARIABLESCOPE_HPP_LOCK
