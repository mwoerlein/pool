#ifndef SYS_STREAM_STREAMFACTORY_HPP_LOCK
#define SYS_STREAM_STREAMFACTORY_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/String.hpp"
#include "sys/stream/SeekableIOStream.hpp"
#include "sys/stream/NullIOStream.hpp"

/*
 * Dummy implementation!
 * Will be replaced in linux-based systems (test, (cross-)compiler, ...) with file-based streams
 */
class StreamFactory: virtual public Object {
    public:
    StreamFactory(Environment &env, MemoryInfo &mi = *notAnInfo): Object(env, mi) {}
    virtual ~StreamFactory() {}
    
    virtual SeekableIOStream & buildOStream(const char * name) { return env().create<NullIOStream>(); }
    virtual SeekableIOStream & buildOStream(String &name) { return env().create<NullIOStream>(); }
    virtual IStream & buildStdIStream() { return env().create<NullIOStream>(); }
    virtual SeekableIOStream & buildIStream(const char * name) { return env().create<NullIOStream>(); }
    virtual SeekableIOStream & buildIStream(String &name) { return env().create<NullIOStream>(); }
};

#endif //SYS_STREAM_STREAMFACTORY_HPP_LOCK
