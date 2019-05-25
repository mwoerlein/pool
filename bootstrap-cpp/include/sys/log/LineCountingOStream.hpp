#ifndef SYS_LOG_LINECOUNTINGOSTREAM_HPP_LOCK
#define SYS_LOG_LINECOUNTINGOSTREAM_HPP_LOCK

#include "sys/stream/OStream.hpp"

class LineCountingOStream: public OStream {
    private:
    int lines;
    OStream &inner;
    
    public:
    LineCountingOStream(Environment &env, MemoryInfo &mi, OStream &inner):Object(env, mi),inner(inner),lines(0) {}
    virtual ~LineCountingOStream() {}
    
    using OStream::operator <<;
    virtual OStream &operator <<(char c) override {
        if (c == '\n') {
            lines++;
        }
        inner << c;
        return *this;
    }
    operator int() {
        return lines;
    }
};

#endif //SYS_LOG_LINECOUNTINGOSTREAM_HPP_LOCK
