#ifndef POOLC_AST_SCOPES_ALLTYPE_HPP_LOCK
#define POOLC_AST_SCOPES_ALLTYPE_HPP_LOCK

#include "poolc/ast/Type.hpp"

class AllType: public Type {
    public:
    AllType(Environment &env, MemoryInfo &mi);
    virtual ~AllType();
    virtual AllType * isAll() override;
};

#endif //POOLC_AST_SCOPES_ALLTYPE_HPP_LOCK
