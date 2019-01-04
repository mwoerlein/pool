#ifndef POOLC_PIR_VALUE_PIRSTRING_HPP_LOCK
#define POOLC_PIR_VALUE_PIRSTRING_HPP_LOCK

#include "poolc/pir/PIRValue.hpp"

#include "sys/String.hpp"

class PIRString: public PIRValue {
    public:
    String &id;
    PIRString(Environment &env, MemoryInfo &mi, String &id);
    virtual ~PIRString();
    virtual PIRString *isString() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_VALUE_PIRSTRING_HPP_LOCK
