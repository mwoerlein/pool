#ifndef INTERRUPTVECTORTABLE_HPP_LOCK
#define INTERRUPTVECTORTABLE_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/InterruptHandler.hpp"

class InterruptVectorTable: virtual public Object {
    public:
    virtual int getSize() = 0;
    virtual void insert(int i, InterruptHandler &hdl) = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
};

#endif //INTERRUPTVECTORTABLE_HPP_LOCK

