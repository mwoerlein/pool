#ifndef POOLC_AST_TYPE_HPP_LOCK
#define POOLC_AST_TYPE_HPP_LOCK

#include "sys/Object.hpp"

class ClassScope;
class InstanceScope;
class IntType;
class CStringType;
class Type: virtual public Object {
    public:
    Type(Environment &env, MemoryInfo &mi);
    virtual ~Type();
    
    virtual ClassScope * isClass();
    virtual InstanceScope * isInstance();
    virtual IntType * isInt();
    virtual CStringType * isCString();
    
    virtual OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_AST_TYPE_HPP_LOCK
