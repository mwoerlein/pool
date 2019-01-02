#ifndef POOLC_PIR_STATEMENT_PIRASM_HPP_LOCK
#define POOLC_PIR_STATEMENT_PIRASM_HPP_LOCK

#include "poolc/pir/PIRStatement.hpp"

#include "sys/String.hpp"

class PIRAsm: public PIRStatement {
    public:
    String &pasm;
    PIRAsm(Environment &env, MemoryInfo &mi, String &pasm);
    virtual ~PIRAsm();
    virtual PIRAsm *isAsm() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_STATEMENT_PIRASM_HPP_LOCK
