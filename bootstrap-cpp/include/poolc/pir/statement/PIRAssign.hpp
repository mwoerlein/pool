#ifndef POOLC_PIR_STATEMENT_PIRASSIGN_HPP_LOCK
#define POOLC_PIR_STATEMENT_PIRASSIGN_HPP_LOCK

#include "poolc/pir/PIRStatement.hpp"

#include "poolc/pir/PIRValue.hpp"
#include "poolc/pir/PIRLocation.hpp"

class PIRAssign: public PIRStatement {
    public:
    PIRValue &val;
    PIRLocation &dest;
    PIRAssign(Environment &env, MemoryInfo &mi, PIRValue &val, PIRLocation &dest);
    virtual ~PIRAssign();
    virtual PIRAssign *isAssign() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_STATEMENT_PIRASSIGN_HPP_LOCK
