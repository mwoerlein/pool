#ifndef POOLC_AST_TYPE_HPP_LOCK
#define POOLC_AST_TYPE_HPP_LOCK

#include "sys/Object.hpp"

class AllType;
class AnyType;
class ClassScope;
class CStringType;
class InstanceScope;
class IntType;
class Type: virtual public Object {
    public:
    Type(Environment &env, MemoryInfo &mi);
    virtual ~Type();
    
    virtual AllType * isAll();
    virtual AnyType * isAny();
    virtual ClassScope * isClass();
    virtual CStringType * isCString();
    virtual InstanceScope * isInstance();
    virtual IntType * isInt();
    inline bool isBool() { return isInt() || isCString() || isInstance() || isAll() || isAny(); };
    
    virtual OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_AST_TYPE_HPP_LOCK
