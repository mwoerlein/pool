#ifndef POOLC_PIR_PIRBASICBLOCK_HPP_LOCK
#define POOLC_PIR_PIRBASICBLOCK_HPP_LOCK

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

enum bb_kind { bb_entry, bb_exit, bb_block };
class PIRBasicBlock: virtual public Object, virtual public LoggerAware {
    private:
    LinkedList<PIRStatement> &_statements;
    
    bool isAssignable(Type &src, Type &dest);
    bool isValidCall(MethodScope &method, Collection<PIRLocation> &params, Collection<PIRLocation> &rets);
    
    public:
    bb_kind kind;
    PIRMethod &method;
    int idx;
    PIRCond *cond;
    PIRBasicBlock *next, *condNext;
    
    PIRBasicBlock(Environment &env, MemoryInfo &mi, bb_kind kind, PIRMethod &method, int idx);
    virtual ~PIRBasicBlock();
    
    inline Iterator<PIRStatement> &statements() { return _statements.iterator(); }
    void setCondNext(logical_op op, PIRLocation &left, PIRLocation &right, PIRBasicBlock &trueNext, PIRBasicBlock &falseNext);
    
    void addArithOp(arith_op op, PIRLocation &left, PIRLocation &right, PIRLocation &dest);
    void addAsm(String &pasm, Map<String, PIRLocation> &in, Map<String, PIRLocation> &out);
    void addAssign(PIRValue &value, PIRLocation &dest, bool reinterpret = false);
    void addCall(PIRLocation &context, MethodScope &method, Collection<PIRLocation> &params, Collection<PIRLocation> &rets);
    void addGlobalCall(MethodScope &method, Collection<PIRLocation> &params, Collection<PIRLocation> &rets);
    void addGet(PIRLocation &context, VariableScope &var, PIRLocation &dest);
    void addMove(PIRLocation &src, PIRLocation &dest, bool reinterpret = false);
    void addSet(PIRLocation &context, VariableScope &var, PIRLocation &src);
    
    virtual OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_PIRBASICBLOCK_HPP_LOCK
