#ifndef SYS_STREAM_ISTREAM_HPP_LOCK
#define SYS_STREAM_ISTREAM_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/Char.hpp"
#include "sys/stream/OStream.hpp"

class IStream: virtual public Object {
    public:
    virtual ~IStream();
    
    virtual bool isEmpty() = 0;
    
    virtual IStream & operator >>(char &c) = 0;
    virtual IStream & operator >>(Char &c);
    virtual OStream & operator >>(OStream & stream) override;
    
    virtual unsigned char readRawChar();
    virtual unsigned int readRawInt();
};

#endif //SYS_STREAM_ISTREAM_HPP_LOCK

