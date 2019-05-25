#include "poolc/pir/statement/PIRAssign.hpp"

// public
PIRAssign::PIRAssign(Environment &env, MemoryInfo &mi, PIRValue &val, PIRLocation &dest)
        :PIRStatement(env, mi), Object(env, mi), val(val), dest(dest) {}
PIRAssign::~PIRAssign() {}
PIRAssign *PIRAssign::isAssign() { return this; }

OStream & PIRAssign::operator >>(OStream & stream) {
    return stream << "ASSIGN(" << val << ", " << dest << ")";
}
