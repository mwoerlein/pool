#ifndef POOLC_PIR_STATEMENT_PIRSET_HPP_LOCK
#define POOLC_PIR_STATEMENT_PIRSET_HPP_LOCK

#include "poolc/pir/PIRStatement.hpp"

#include "poolc/ast/scopes/VariableScope.hpp"
#include "poolc/pir/PIRLocation.hpp"

class PIRSet: public PIRStatement {
    public:
    PIRLocation &context;
    VariableScope &variable;
    PIRLocation &src;
    PIRSet(Environment &env, MemoryInfo &mi, PIRLocation &context, VariableScope &variable, PIRLocation &src);
    virtual ~PIRSet();
    virtual PIRSet *isSet() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_STATEMENT_PIRSET_HPP_LOCK
