#ifndef POOLC_PIR_VALUE_PIRNULL_HPP_LOCK
#define POOLC_PIR_VALUE_PIRNULL_HPP_LOCK

#include "poolc/pir/PIRValue.hpp"

class PIRNull: public PIRValue {
    public:
    PIRNull(Environment &env, MemoryInfo &mi);
    virtual ~PIRNull();
    virtual PIRNull *isNull() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_VALUE_PIRNULL_HPP_LOCK
