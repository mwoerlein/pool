#ifndef POOLC_AST_SCOPES_NAMEDTYPE_HPP_LOCK
#define POOLC_AST_SCOPES_NAMEDTYPE_HPP_LOCK

#include "poolc/ast/Type.hpp"

class NamedType: public Type {
    public:
    NamedType(Environment &env, MemoryInfo &mi);
    virtual ~NamedType();
};

#endif //POOLC_AST_SCOPES_NAMEDTYPE_HPP_LOCK
