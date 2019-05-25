#include "poolc/pir/value/PIRNull.hpp"

// public
PIRNull::PIRNull(Environment &env, MemoryInfo &mi):PIRValue(env, mi), Object(env, mi) {}
PIRNull::~PIRNull() {}
PIRNull *PIRNull::isNull() { return this; }

OStream & PIRNull::operator >>(OStream & stream) {
    return stream << "null";
}
