#include "sys/stream/IStream.hpp"

// public
IStream::~IStream() {};

IStream & IStream::operator >>(Char &c) {
    char tmp;
    *this >> tmp;
    c = tmp;
    return *this;
}

OStream & IStream::operator >>(OStream & stream) {
    char tmp;
    while (!isEmpty()) {
        *this >> tmp;
        stream << tmp;
    }
    return stream;
}

unsigned char IStream::readRawChar() {
    char tmp;
    *this >> tmp;
    return tmp;
}

unsigned int IStream::readRawInt() {
    return (readRawChar()) + (readRawChar()<<8) + (readRawChar()<<16) + (readRawChar()<<24);
}
