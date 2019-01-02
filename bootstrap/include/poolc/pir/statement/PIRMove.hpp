#ifndef POOLC_PIR_STATEMENT_PIRMOVE_HPP_LOCK
#define POOLC_PIR_STATEMENT_PIRMOVE_HPP_LOCK

#include "poolc/pir/PIRStatement.hpp"

#include "poolc/pir/PIRLocation.hpp"

class PIRMove: public PIRStatement {
    public:
    PIRLocation &src;
    PIRLocation &dest;
    PIRMove(Environment &env, MemoryInfo &mi, PIRLocation &src, PIRLocation &dest);
    virtual ~PIRMove();
    virtual PIRMove *isMove() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_STATEMENT_PIRMOVE_HPP_LOCK
