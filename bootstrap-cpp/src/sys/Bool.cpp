#include "sys/Bool.hpp"

// public
Bool::Bool(Environment &env, MemoryInfo &mi, bool b): Object(env, mi), _b(b) {}
Bool::Bool(Environment &env, MemoryInfo &mi, Bool &b): Object(env, mi), _b((bool) b) {}
Bool::~Bool() {};

Bool::operator bool() {
    return _b;
}

Bool & Bool::operator =(bool b) {
    _b = b;
    return *this;
}

Bool & Bool::operator =(Bool &b) {
    _b = (bool) b;
    return *this;
}

OStream & Bool::operator >>(OStream & stream) {
    return stream << (_b ? "true" : "false");
}

int Bool::hash() {
    return (int) _b;
}

RTTI Bool::rtti() {
    return boolean;
}

bool Bool::equals(Object &o) {
    if (Bool *b = env().as<Bool>(o, rtti())) {
        return _b == (bool) b->_b;
    }
    return false;
}
