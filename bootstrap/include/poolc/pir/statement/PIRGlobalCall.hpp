#ifndef POOLC_PIR_STATEMENT_PIRGLOBALCALL_HPP_LOCK
#define POOLC_PIR_STATEMENT_PIRGLOBALCALL_HPP_LOCK

#include "poolc/pir/PIRStatement.hpp"

#include "sys/collection/Collection.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"
#include "poolc/pir/PIRLocation.hpp"

class PIRGlobalCall: public PIRStatement {
    public:
    MethodScope &method;
    Collection<PIRLocation> &params;
    Collection<PIRLocation> &rets;
    PIRGlobalCall(Environment &env, MemoryInfo &mi, MethodScope &method, Collection<PIRLocation> &params, Collection<PIRLocation> &rets);
    virtual ~PIRGlobalCall();
    virtual PIRGlobalCall *isGlobalCall() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_STATEMENT_PIRGLOBALCALL_HPP_LOCK
