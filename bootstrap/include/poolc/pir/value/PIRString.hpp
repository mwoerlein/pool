#ifndef POOLC_PIR_VALUE_PIRSTRING_HPP_LOCK
#define POOLC_PIR_VALUE_PIRSTRING_HPP_LOCK

#include "poolc/pir/PIRValue.hpp"

#include "poolc/ast/scopes/VariableScope.hpp"

class PIRString: public PIRValue {
    public:
    VariableScope &constant;
    PIRString(Environment &env, MemoryInfo &mi, VariableScope &constant);
    virtual ~PIRString();
    virtual PIRString *isString() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_VALUE_PIRSTRING_HPP_LOCK
