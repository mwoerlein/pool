#include "poolc/pir/PIRValue.hpp"

// public
PIRValue::PIRValue(Environment &env, MemoryInfo &mi): Object(env, mi) {}
PIRValue::~PIRValue() {}

PIRInt *PIRValue::isInt() { return 0; }
PIRNull *PIRValue::isNull() { return 0; }
PIRString *PIRValue::isString() { return 0; }
