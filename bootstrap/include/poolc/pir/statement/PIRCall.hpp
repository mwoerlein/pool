#ifndef POOLC_PIR_STATEMENT_PIRCALL_HPP_LOCK
#define POOLC_PIR_STATEMENT_PIRCALL_HPP_LOCK

#include "poolc/pir/PIRStatement.hpp"

#include "sys/collection/Collection.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"
#include "poolc/pir/PIRLocation.hpp"

class PIRCall: public PIRStatement {
    public:
    PIRLocation &context;
    MethodScope &method;
    Collection<PIRLocation> &params;
    Collection<PIRLocation> &rets;
    PIRCall(Environment &env, MemoryInfo &mi, PIRLocation &context, MethodScope &method, Collection<PIRLocation> &params, Collection<PIRLocation> &rets);
    virtual ~PIRCall();
    virtual PIRCall *isCall() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_STATEMENT_PIRCALL_HPP_LOCK
