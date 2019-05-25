#ifndef INTERRUPTHANDLER_HPP_LOCK
#define INTERRUPTHANDLER_HPP_LOCK

#include "sys/Object.hpp"

class InterruptHandler: virtual public Object {
    public:
    virtual void call(int nr, int errorCode) = 0;
};

#endif //INTERRUPTHANDLER_HPP_LOCK

