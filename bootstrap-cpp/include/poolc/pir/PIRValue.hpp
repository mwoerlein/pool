#ifndef POOLC_PIR_PIRVALUE_HPP_LOCK
#define POOLC_PIR_PIRVALUE_HPP_LOCK

#include "sys/Object.hpp"

class PIRInt;
class PIRNull;
class PIRString;
class PIRValue: virtual public Object {
    public:
    PIRValue(Environment &env, MemoryInfo &mi);
    virtual ~PIRValue();
    
    virtual PIRInt *isInt();
    virtual PIRNull *isNull();
    virtual PIRString *isString();
};

#endif //POOLC_PIR_PIRVALUE_HPP_LOCK
