#include "poolc/pir/value/PIRInt.hpp"

// public
PIRInt::PIRInt(Environment &env, MemoryInfo &mi, int value)
        :PIRValue(env, mi), Object(env, mi), value(value) {}
PIRInt::~PIRInt() {}
PIRInt *PIRInt::isInt() { return this; }

OStream & PIRInt::operator >>(OStream & stream) {
    return stream << "const_i(" << value << ")";
}
