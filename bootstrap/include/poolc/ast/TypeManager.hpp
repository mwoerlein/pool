#ifndef POOLC_AST_TYPEMANAGER_HPP_LOCK
#define POOLC_AST_TYPEMANAGER_HPP_LOCK

#include "sys/Object.hpp"

#include "poolc/ast/scopes/AllType.hpp"
#include "poolc/ast/scopes/AnyType.hpp"
#include "poolc/ast/scopes/CStringType.hpp"
#include "poolc/ast/scopes/IntType.hpp"

class TypeManager: virtual public Object {
    public:
    AllType & allType;
    AnyType & anyType;
    CStringType & cStringType;
    IntType & intType;
    
    TypeManager(Environment &env, MemoryInfo &mi);
    virtual ~TypeManager();
};

#endif //POOLC_AST_TYPEMANAGER_HPP_LOCK
