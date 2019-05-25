#ifndef SYS_STREAM_SEEKABLEIOSTREAM_HPP_LOCK
#define SYS_STREAM_SEEKABLEIOSTREAM_HPP_LOCK

#include "sys/stream/IStream.hpp"
#include "sys/stream/OStream.hpp"

class SeekableIOStream: public IStream, public OStream {
    public:
    virtual ~SeekableIOStream();
    
    virtual size_t length() = 0;
    virtual size_t pos() = 0;
    virtual void seek(size_t pos) = 0;
    
    virtual void rewind();
};

#endif //SYS_STREAM_SEEKABLEIOSTREAM_HPP_LOCK
