#include "sys/stream/OStream.hpp"

// public
OStream::~OStream() {};

OStream & OStream::operator <<(int i) {
    return printint(i);
}

OStream & OStream::operator <<(unsigned int i) {
    return printuint(i);
}

OStream & OStream::operator <<(void * ptr) {
    return printuhex((unsigned int)ptr, 8);
}

OStream & OStream::operator <<(char * c) {
    while (*c) {
        *this << *c++;
    }
    return *this;
}

OStream & OStream::operator <<(const char * c) {
    return *this << (char *) c;
}

OStream & OStream::operator <<(Object &o) {
    return o >> *this;
}

OStream & OStream::printint(int d, int base, int pad) {
    /*  If D is minus, put `-' in the head. */
    if (d < 0) {
        *this << '-';
        d = -d;
    }
    return printuint((unsigned int) d, base, pad);
}

OStream & OStream::printuint(unsigned int d, int base, int pad) {
    char buf[20];
    char *p = buf;

    /*  Divide D by BASE until D == 0. */
    do {
        int remainder = d % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while (d /= base);
    while (buf+pad > p) {
        *p++ = '0';
    }
    
    /* print reverse buffer */
    while (p > buf) {
        *this << *(--p);
    }

    return *this;
}

OStream & OStream::printhex(int d, int pad) {
    /*  If D is minus, put `-' in the head. */
    if (d < 0) {
        *this << '-';
        d = -d;
    }
    return printuhex((unsigned int) d, pad);
}

OStream & OStream::printuhex(unsigned int d, int pad) {
    *this << '0' << 'x';
    return printuint(d, 16, pad);
}

OStream & OStream::printRawChar(unsigned char c) {
    return *this << (char) c;
}

OStream & OStream::printRawInt(unsigned int i) {
    return *this << (char)i << (char)(i>>8) << (char)(i>>16) << (char)(i>>24);
}
