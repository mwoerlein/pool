#ifndef POOLC_PIR_PIRSTATEMENT_HPP_LOCK
#define POOLC_PIR_PIRSTATEMENT_HPP_LOCK

#include "sys/Object.hpp"

class PIRAssign;
class PIRCall;
class PIRGet;
class PIRMove;
class PIRReturn;
class PIRSet;
class PIRStatement: virtual public Object {
    public:
    PIRStatement(Environment &env, MemoryInfo &mi);
    virtual ~PIRStatement();

    virtual PIRAssign *isAssign();
    virtual PIRCall *isCall();
    virtual PIRGet *isGet();
    virtual PIRMove *isMove();
    virtual PIRReturn *isReturn();
    virtual PIRSet *isSet();
};

#endif //POOLC_PIR_PIRSTATEMENT_HPP_LOCK
