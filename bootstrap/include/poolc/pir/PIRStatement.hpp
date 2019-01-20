#ifndef POOLC_PIR_PIRSTATEMENT_HPP_LOCK
#define POOLC_PIR_PIRSTATEMENT_HPP_LOCK

#include "sys/Object.hpp"

class PIRArithOp;
class PIRAsm;
class PIRAssign;
class PIRCall;
class PIRCond;
class PIRGet;
class PIRMove;
class PIRSet;
class PIRStatement: virtual public Object {
    public:
    PIRStatement(Environment &env, MemoryInfo &mi);
    virtual ~PIRStatement();

    virtual PIRArithOp *isArithOp();
    virtual PIRAsm *isAsm();
    virtual PIRAssign *isAssign();
    virtual PIRCall *isCall();
    virtual PIRCond *isCond();
    virtual PIRGet *isGet();
    virtual PIRMove *isMove();
    virtual PIRSet *isSet();
};

#endif //POOLC_PIR_PIRSTATEMENT_HPP_LOCK
