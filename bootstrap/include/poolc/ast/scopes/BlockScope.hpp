#ifndef POOLC_AST_SCOPES_BLOCKSCOPE_HPP_LOCK
#define POOLC_AST_SCOPES_BLOCKSCOPE_HPP_LOCK

#include "poolc/ast/Scope.hpp"

#include "poolc/ast/nodes/instruction/BlockInstNode.hpp"

class BlockScope: public Scope {
    private:
    BlockInstNode & block;
    
    public:
    BlockScope(Environment &env, MemoryInfo &mi, Scope & parent, BlockInstNode & block);
    virtual ~BlockScope();
    virtual BlockScope * isBlock() override;
    virtual BlockScope * getBlock() override;
    virtual BlockInstNode * getBlockInstNode() override;

    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_SCOPES_BLOCKSCOPE_HPP_LOCK
