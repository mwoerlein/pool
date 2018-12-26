#ifndef POOLC_AST_SCOPES_BLOCKSCOPE_HPP_LOCK
#define POOLC_AST_SCOPES_BLOCKSCOPE_HPP_LOCK

#include "poolc/ast/Scope.hpp"

#include "poolc/ast/nodes/instruction/BlockInstNode.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"

class BlockScope: public Scope {
    private:
    BlockInstNode & block;
    
    public:
    BlockScope(Environment &env, MemoryInfo &mi, BlockInstNode & block, MethodScope & parent);
    virtual ~BlockScope();
    virtual BlockScope * isBlock() override;
    virtual BlockInstNode * getBlockInstNode() override;
};

#endif //POOLC_AST_SCOPES_BLOCKSCOPE_HPP_LOCK
