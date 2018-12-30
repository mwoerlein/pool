#ifndef POOLC_AST_SCOPES_CSTRINGTYPE_HPP_LOCK
#define POOLC_AST_SCOPES_CSTRINGTYPE_HPP_LOCK

#include "poolc/ast/Type.hpp"

class CStringType: public Type {
    public:
    CStringType(Environment &env, MemoryInfo &mi);
    virtual ~CStringType();
    virtual CStringType * isCString() override;
};

#endif //POOLC_AST_SCOPES_CSTRINGTYPE_HPP_LOCK
