#include "sys/Char.hpp"

// public
Char::Char(Environment &env, MemoryInfo &mi, char c): Object(env, mi), _c(c) {}
Char::Char(Environment &env, MemoryInfo &mi, Char &c): Object(env, mi), _c((char) c) {}
Char::~Char() {};

Char::operator char() {
    return _c;
}

Char & Char::operator =(char c) {
    _c = c;
    return *this;
}

Char & Char::operator =(Char &c) {
    _c = (char) c;
    return *this;
}

int Char::compareTo(char c) {
    return _c - c;
}

int Char::compareTo(Char &c) {
    return _c - (char) c;
}

OStream & Char::operator >>(OStream & stream) {
    return stream << _c;
}

int Char::hash() {
    return _c;
}

RTTI Char::rtti() {
    return character;
}
