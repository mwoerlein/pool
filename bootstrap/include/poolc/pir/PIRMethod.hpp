#ifndef POOLC_PIR_PIRMETHOD_HPP_LOCK
#define POOLC_PIR_PIRMETHOD_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/log/LoggerAware.hpp"

#include "sys/collection/LinkedList.hpp"
#include "poolc/Arith.hpp"
#include "poolc/ast/nodes/expression/ConstCStringExprNode.hpp"
#include "poolc/ast/nodes/expression/ConstIntExprNode.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"
#include "poolc/ast/scopes/VariableScope.hpp"

#include "poolc/pir/PIRLocation.hpp"
#include "poolc/pir/PIRStatement.hpp"
#include "poolc/pir/value/PIRNull.hpp"
#include "poolc/pir/value/PIRInt.hpp"
#include "poolc/pir/value/PIRString.hpp"

/*
    frame-location:
         ret(X)
         param(X)
         spilled(X)
         temp(X)
         this (invalid in global methods!)
    value:
         null
         const_i(X)
         const_s(ID)
    statement:
         ASSIGN(value, temp(X))
         MOVE(location, temp(X))
         MOVE(temp(X), location)
         GET(temp(X), varID, temp(Y))
         SET(temp(X), varID, temp(Y))
         CALL(temp(X), methID, [temp(A0),...], [temp(R0),...])
         RETURN
*/
class PIRMethod: virtual public Object, virtual public LoggerAware {
    private:
    MethodScope *_scope = 0;
    PIRLocation *_this = 0;
    PIRLocation *_thisTemp = 0;
    PIRLocation *_zeroTemp = 0;
    PIRLocation *_oneTemp = 0;
    PIRNull &_null;
    LinkedList<PIRLocation> &_params;
    LinkedList<PIRLocation> &_rets;
    LinkedList<PIRLocation> &_spills;
    LinkedList<PIRLocation> &_temps;
    LinkedList<PIRStatement> &_statements;
    
    PIRLocation *newLocation(location_kind kind, Type &type);
    bool isAssignable(Type &src, Type &dest);
    
    public:
    PIRMethod(Environment &env, MemoryInfo &mi);
    virtual ~PIRMethod();
    
    void init(MethodScope &scope);
    inline MethodScope &scope() { return *_scope; }
    
    inline bool hasThis() { return _this; }
    inline int paramCount() { return _params.size(); }
    inline int retCount() { return _rets.size(); }
    inline int spillCount() { return _spills.size(); }
    inline int tempCount() { return _temps.size(); }
    
    inline PIRLocation *getThis() { return _this; }
    inline PIRLocation *getParam(int idx) { return _params.at(idx); }
    inline PIRLocation *getRet(int idx) { return _rets.at(idx); }
    inline PIRLocation *getSpill(int idx) { return _spills.at(idx); }
    inline PIRLocation *getTemp(int idx) { return _temps.at(idx); }
    
    inline PIRNull &getNull() { return _null; }
    inline PIRInt &getConstInt(ConstIntExprNode &value) { return env().create<PIRInt, int>(value.value); }
    PIRString &getConstString(ConstCStringExprNode &value);
    PIRLocation *getZeroTemp(Type &type);
    PIRLocation *getOneTemp(Type &type);
    
    inline Iterator<PIRStatement> &statements() { return _statements.iterator(); }
    
    inline PIRLocation &newTemp(Type &type) { return *newLocation(loc_temp, type); }
    inline PIRLocation &asTemp(PIRLocation &src) {
        // TODO #11: treat all locations as "temps" until register allocations seperates frame and register locations
        return src;
/*        
        if (src.kind == loc_temp) { return src; }
        if (&src == _this) { return *_thisTemp; }
        PIRLocation &tmp = *newLocation(loc_temp, src.type);
        addMove(src, tmp);
        return tmp;
*/
    }
    PIRLocation &spillTemp(int idx);
    
    void addArithOp(arith_op op, PIRLocation &left, PIRLocation &right, PIRLocation &dest);
    void addAsm(String &pasm, Map<String, PIRLocation> &in, Map<String, PIRLocation> &out);
    void addAssign(PIRValue &value, PIRLocation &dest);
    void addCall(PIRLocation &context, MethodScope &method, Collection<PIRLocation> &params, Collection<PIRLocation> &rets);
    void addGet(PIRLocation &context, VariableScope &var, PIRLocation &dest);
    void addMove(PIRLocation &src, PIRLocation &dest, bool reinterpret = false);
    void addReturn();
    void addSet(PIRLocation &context, VariableScope &var, PIRLocation &src);
};

#endif //POOLC_PIR_PIRMETHOD_HPP_LOCK
