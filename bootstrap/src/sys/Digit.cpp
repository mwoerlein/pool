#include "sys/Digit.hpp"

// public
Digit::Digit(Environment &env, MemoryInfo &mi, char c): Char(env, mi, ctod(c)), Object(env, mi) {}
Digit::Digit(Environment &env, MemoryInfo &mi, Char &c): Char(env, mi, ctod((char) c)), Object(env, mi) {}
Digit::Digit(Environment &env, MemoryInfo &mi, int i): Char(env, mi, itod(i)), Object(env, mi) {}
Digit::Digit(Environment &env, MemoryInfo &mi, Integer &i): Char(env, mi, itod((int) i)), Object(env, mi) {}
Digit::~Digit() {}

Digit::operator int() {
    return ctoi(_c);
}

Digit & Digit::operator =(char c) {
    Char::_c = ctod(c);
    return *this;
}

Digit & Digit::operator =(Char &c) {
    Char::_c = ctod((char) c);
    return *this;
}

Digit & Digit::operator =(Digit &d) {
    Char::_c = (char) d;
    return *this;
}

Digit & Digit::operator =(int i) {
    Char::_c = itod(i);
    return *this;
}

Digit & Digit::operator =(Integer &i) {
    Char::_c = itod((int) i);
    return *this;
}
