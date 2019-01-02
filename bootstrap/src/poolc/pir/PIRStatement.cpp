#include "poolc/pir/PIRStatement.hpp"

// public
PIRStatement::PIRStatement(Environment &env, MemoryInfo &mi): Object(env, mi) {}
PIRStatement::~PIRStatement() {}

PIRAsm *PIRStatement::isAsm() { return 0; }
PIRAssign *PIRStatement::isAssign() { return 0; }
PIRCall *PIRStatement::isCall() { return 0; }
PIRGet *PIRStatement::isGet() { return 0; }
PIRMove *PIRStatement::isMove() { return 0; }
PIRReturn *PIRStatement::isReturn() { return 0; }
PIRSet *PIRStatement::isSet() { return 0; }
