#include "poolc/pir/statement/PIRReturn.hpp"

// public
PIRReturn::PIRReturn(Environment &env, MemoryInfo &mi):PIRStatement(env, mi), Object(env, mi) {}
PIRReturn::~PIRReturn() {}
PIRReturn *PIRReturn::isReturn() { return this; }

OStream & PIRReturn::operator >>(OStream & stream) {
    return stream << "RETURN";
}
