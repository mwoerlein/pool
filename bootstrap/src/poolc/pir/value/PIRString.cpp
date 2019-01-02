#include "poolc/pir/value/PIRString.hpp"

// public
PIRString::PIRString(Environment &env, MemoryInfo &mi, VariableScope &constant)
        :PIRValue(env, mi), Object(env, mi), constant(constant) {}
PIRString::~PIRString() {}
PIRString *PIRString::isString() { return this; }

OStream & PIRString::operator >>(OStream & stream) {
    return stream << "const_s('" << constant.getVariableDeclNode()->name << "')";
}
