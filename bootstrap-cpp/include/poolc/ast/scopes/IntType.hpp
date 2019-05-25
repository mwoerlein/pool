#ifndef POOLC_AST_SCOPES_INTTYPE_HPP_LOCK
#define POOLC_AST_SCOPES_INTTYPE_HPP_LOCK

#include "poolc/ast/Type.hpp"

class IntType: public Type {
    public:
    IntType(Environment &env, MemoryInfo &mi);
    virtual ~IntType();
    virtual IntType * isInt() override;
};

#endif //POOLC_AST_SCOPES_INTTYPE_HPP_LOCK
