#ifndef POOLC_AST_SCOPES_ANYTYPE_HPP_LOCK
#define POOLC_AST_SCOPES_ANYTYPE_HPP_LOCK

#include "poolc/ast/Type.hpp"

class AnyType: public Type {
    public:
    AnyType(Environment &env, MemoryInfo &mi);
    virtual ~AnyType();
    virtual AnyType * isAny() override;
};

#endif //POOLC_AST_SCOPES_ANYTYPE_HPP_LOCK
