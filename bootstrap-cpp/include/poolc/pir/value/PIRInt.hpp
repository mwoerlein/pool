#ifndef POOLC_PIR_VALUE_PIRINT_HPP_LOCK
#define POOLC_PIR_VALUE_PIRINT_HPP_LOCK

#include "poolc/pir/PIRValue.hpp"

class PIRInt: public PIRValue {
    public:
    int value;
    PIRInt(Environment &env, MemoryInfo &mi, int value);
    virtual ~PIRInt();
    virtual PIRInt *isInt() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_VALUE_PIRINT_HPP_LOCK
