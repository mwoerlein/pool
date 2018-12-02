#include "sys/Integer.hpp"

// public
Integer::Integer(Environment &env, MemoryInfo &mi, int i): Object(env, mi), _i(i) {}
Integer::Integer(Environment &env, MemoryInfo &mi, Integer &i): Object(env, mi), _i((int) i) {}
Integer::~Integer() {}
    
Integer::operator int() {
    return _i;
}

Integer & Integer::operator =(int i) {
    _i = i;
    return *this;
}

Integer & Integer::operator =(Integer &i) {
    _i = (int) i;
    return *this;
}

Integer & Integer::operator ++() {
    ++_i;
    return *this;
}

Integer & Integer::operator --() {
    --_i;
    return *this;
}

Integer & Integer::operator +=(int i) {
    _i += i;
    return *this;
}

Integer & Integer::operator -=(int i) {
    _i -= i;
    return *this;
}

Integer & Integer::operator *=(int i) {
    _i *= i;
    return *this;
}

Integer & Integer::operator /=(int i) {
    _i /= i;
    return *this;
}

Integer & Integer::operator %=(int i) {
    _i %= i;
    return *this;
}

Integer & Integer::operator +=(Integer &i) {
    _i += (int) i;
    return *this;
}

Integer & Integer::operator -=(Integer &i) {
    _i -= (int) i;
    return *this;
}

Integer & Integer::operator *=(Integer &i) {
    _i *= (int) i;
    return *this;
}

Integer & Integer::operator /=(Integer &i) {
    _i /= (int) i;
    return *this;
}
Integer & Integer::operator %=(Integer &i) {
    _i %= (int) i;
    return *this;
}

int Integer::compareTo(int i) {
    return _i - i;
}

int Integer::compareTo(Integer &i) {
    return _i - (int) i;
}

OStream & Integer::operator >>(OStream & stream) {
    return stream << _i;
}

int Integer::hash() {
    return _i;
}

RTTI Integer::rtti() {
    return integer;
}
