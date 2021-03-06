#include "poolc/pir/PIRStatement.hpp"

// public
PIRStatement::PIRStatement(Environment &env, MemoryInfo &mi): Object(env, mi) {}
PIRStatement::~PIRStatement() {}

PIRArithOp *PIRStatement::isArithOp() { return 0; }
PIRAsm *PIRStatement::isAsm() { return 0; }
PIRAssign *PIRStatement::isAssign() { return 0; }
PIRCall *PIRStatement::isCall() { return 0; }
PIRGlobalCall *PIRStatement::isGlobalCall() { return 0; }
PIRCond *PIRStatement::isCond() { return 0; }
PIRGet *PIRStatement::isGet() { return 0; }
PIRMove *PIRStatement::isMove() { return 0; }
PIRSet *PIRStatement::isSet() { return 0; }
