#ifndef POOLC_PIR_STATEMENT_PIRARITHOP_HPP_LOCK
#define POOLC_PIR_STATEMENT_PIRARITHOP_HPP_LOCK

#include "poolc/pir/PIRStatement.hpp"

#include "poolc/Arith.hpp"
#include "poolc/pir/PIRLocation.hpp"

class PIRArithOp: public PIRStatement {
    public:
    arith_op op;
    PIRLocation &left;
    PIRLocation &right;
    PIRLocation &dest;
    PIRArithOp(Environment &env, MemoryInfo &mi, arith_op op, PIRLocation &left, PIRLocation &right, PIRLocation &dest);
    virtual ~PIRArithOp();
    virtual PIRArithOp *isArithOp() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_STATEMENT_PIRARITHOP_HPP_LOCK
