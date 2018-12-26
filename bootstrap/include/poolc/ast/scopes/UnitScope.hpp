#ifndef POOLC_AST_SCOPES_UNITSCOPE_HPP_LOCK
#define POOLC_AST_SCOPES_UNITSCOPE_HPP_LOCK

#include "poolc/ast/Scope.hpp"

#include "poolc/ast/nodes/TranslationUnitNode.hpp"

class UnitScope: public Scope {
    private:
    TranslationUnitNode & unit;
    
    public:
    UnitScope(Environment &env, MemoryInfo &mi, TranslationUnitNode & unit);
    virtual ~UnitScope();
    virtual UnitScope * isUnit() override;
    virtual TranslationUnitNode * getUnitNode() override;
};

#endif //POOLC_AST_SCOPES_UNITSCOPE_HPP_LOCK
