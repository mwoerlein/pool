#include "poolc/pir/value/PIRString.hpp"

// public
PIRString::PIRString(Environment &env, MemoryInfo &mi, String &id)
        :PIRValue(env, mi), Object(env, mi), id(id) {}
PIRString::~PIRString() {}
PIRString *PIRString::isString() { return this; }

OStream & PIRString::operator >>(OStream & stream) {
    return stream << "const_s('" << id << "')";
}
