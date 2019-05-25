#ifndef OBJECT_HPP_LOCK
#define OBJECT_HPP_LOCK

#include "memory/MemoryTypes.hpp"
class Environment;
class OStream;

// TODO: implement RTTI correctly
enum RTTI { object, boolean, character, integer, string, asm_operand };

class Object {
    private:
    Environment &_environment;
    
    protected:
    MemoryInfo *_memory_info;
    friend class Environment;
    
    public:
    Object(Environment &env, MemoryInfo &mi);
    virtual ~Object();
    virtual void destroy();
    
    Environment &env();
    virtual int hash();
    virtual bool equals(Object &o);
    // TODO: implement RTTI correctly
    virtual RTTI rtti();
    virtual OStream & operator >>(OStream & stream);
};

#include "sys/Environment.hpp"
#include "sys/stream/OStream.hpp"

#endif //OBJECT_HPP_LOCK

