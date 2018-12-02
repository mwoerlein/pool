#ifndef SYS_STREAM_OSTREAM_HPP_LOCK
#define SYS_STREAM_OSTREAM_HPP_LOCK

#include "sys/Object.hpp"

class OStream: virtual public Object {
    public:
    virtual ~OStream();
    
    virtual OStream &operator <<(char c) = 0;
    virtual OStream &operator <<(int i);
    virtual OStream &operator <<(unsigned int i);
    virtual OStream &operator <<(void *ptr);
    virtual OStream &operator <<(char *c);
    virtual OStream &operator <<(const char *c);
    
    virtual OStream &operator <<(Object &o);
    
    OStream &printint(int d, int base = 10, int pad = 0);
    OStream &printuint(unsigned int d, int base = 10, int pad = 0);
    OStream &printhex(int d, int pad = 0);
    OStream &printuhex(unsigned int d, int pad = 0);
    
    OStream &printRawChar(unsigned char c);
    OStream &printRawInt(unsigned int d);
};

#endif //SYS_STREAM_OSTREAM_HPP_LOCK

