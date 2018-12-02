#ifndef BOOL_HPP_LOCK
#define BOOL_HPP_LOCK

#include "sys/Object.hpp"

class Bool: public Object {
    private:
    bool _b;
    
    public:
    Bool(Environment &env, MemoryInfo &mi = *notAnInfo, bool b = false);
    Bool(Environment &env, MemoryInfo &mi, Bool &b);
    virtual ~Bool();
    
    operator bool();
    virtual Bool &operator =(Bool &b);
    virtual Bool &operator =(bool b);
    
    virtual OStream & operator >>(OStream & stream) override;
    virtual int hash() override;
    virtual bool equals(Object &o) override;
    
    // TODO: implement RTTI correctly
    virtual RTTI rtti() override;
};

#endif //BOOL_HPP_LOCK

