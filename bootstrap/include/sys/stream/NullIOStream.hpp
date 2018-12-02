#ifndef SYS_STREAM_NULLIOSTREAM_HPP_LOCK
#define SYS_STREAM_NULLIOSTREAM_HPP_LOCK

#include "sys/stream/SeekableIOStream.hpp"

class NullIOStream: public SeekableIOStream {
    public:
    NullIOStream(Environment &env, MemoryInfo &mi = *notAnInfo):Object(env, mi) {}
    virtual ~NullIOStream() {}
    
    using OStream::operator <<;
    virtual OStream &operator <<(char c) override { return *this; }
    using IStream::operator >>;
    virtual IStream & operator >>(char &c) override { c = 0; return *this; }
    virtual bool isEmpty() override { return true; }
    virtual size_t length() override { return 0; }
    virtual size_t pos() override { return 0; }
    virtual void seek(size_t pos) override {}
};

#endif //SYS_STREAM_NULLIOSTREAM_HPP_LOCK

