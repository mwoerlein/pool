#ifndef POOLC_PIR_STATEMENT_PIRCOND_HPP_LOCK
#define POOLC_PIR_STATEMENT_PIRCOND_HPP_LOCK

#include "poolc/pir/PIRStatement.hpp"

#include "poolc/Arith.hpp"
#include "poolc/pir/PIRLocation.hpp"

class PIRCond: public PIRStatement {
    public:
    logical_op op;
    PIRLocation &left;
    PIRLocation &right;
    PIRCond(Environment &env, MemoryInfo &mi, logical_op op, PIRLocation &left, PIRLocation &right);
    virtual ~PIRCond();
    virtual PIRCond *isCond() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_STATEMENT_PIRCOND_HPP_LOCK
