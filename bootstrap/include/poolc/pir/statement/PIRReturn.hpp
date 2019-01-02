#ifndef POOLC_PIR_STATEMENT_PIRRETURN_HPP_LOCK
#define POOLC_PIR_STATEMENT_PIRRETURN_HPP_LOCK

#include "poolc/pir/PIRStatement.hpp"

class PIRReturn: public PIRStatement {
    public:
    PIRReturn(Environment &env, MemoryInfo &mi);
    virtual ~PIRReturn();
    virtual PIRReturn *isReturn() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_STATEMENT_PIRRETURN_HPP_LOCK
