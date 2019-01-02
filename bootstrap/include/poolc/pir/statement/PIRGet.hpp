#ifndef POOLC_PIR_STATEMENT_PIRGET_HPP_LOCK
#define POOLC_PIR_STATEMENT_PIRGET_HPP_LOCK

#include "poolc/pir/PIRStatement.hpp"

#include "poolc/ast/scopes/VariableScope.hpp"
#include "poolc/pir/PIRLocation.hpp"

class PIRGet: public PIRStatement {
    public:
    PIRLocation &context;
    VariableScope &variable;
    PIRLocation &dest;
    PIRGet(Environment &env, MemoryInfo &mi, PIRLocation &context, VariableScope &variable, PIRLocation &dest);
    virtual ~PIRGet();
    virtual PIRGet *isGet() override;
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_STATEMENT_PIRGET_HPP_LOCK
