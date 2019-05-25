#ifndef POOLC_PIR_STATEMENT_PIRASM_HPP_LOCK
#define POOLC_PIR_STATEMENT_PIRASM_HPP_LOCK

#include "poolc/pir/PIRStatement.hpp"

#include "sys/String.hpp"
#include "sys/collection/Map.hpp"
#include "poolc/pir/PIRLocation.hpp"

class PIRAsm: public PIRStatement {
    public:
    String &pasm;
    Map<String, PIRLocation> &in;
    Map<String, PIRLocation> &out;
    PIRAsm(Environment &env, MemoryInfo &mi, String &pasm, Map<String, PIRLocation> &in, Map<String, PIRLocation> &out);
    virtual ~PIRAsm();
    virtual PIRAsm *isAsm() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_STATEMENT_PIRASM_HPP_LOCK
