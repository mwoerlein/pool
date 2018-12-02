#include "sys/Environment.hpp"

#include "sys/stream/StreamFactory.hpp"

void* operator new (size_t size, void* location) {
    return location;
}
//*/
#include "memory/MemoryInfoHelper.hpp"
OStream &operator<<(OStream & out, MemoryInfo * e) {
    out << "MemoryInfo "
               <<(void *) e->buf<<':'<<memoryEnd(e->buf, e->len)<<'['<<(void *) e->len<<']'
               <<" ("<<(void *) e->prev<<'<'<<(void *) e<<'>'<<(void *)e->next<<")";
}
//*/

Environment::Environment():Object(*this, *notAnInfo),ma(0),_out(0),_err(0),_factory(0) {}
Environment::Environment(Environment &env, MemoryInfo &mi, MemoryAllocator &ma, OStream &out, OStream &err)
    :Object(env, mi),ma(&ma),_out(&out),_err(&err),_factory(0) {}
Environment::~Environment() {}

OStream & Environment::out() {
    return *_out;
}

OStream & Environment::setOut(OStream & out) {
    OStream *old = _out;
    _out = &out;
    return *old;
}

OStream & Environment::err() {
    return *_err;
}

OStream & Environment::setErr(OStream & err) {
    OStream *old = _err;
    _err = &err;
    return *old;
}

StreamFactory & Environment::streamFactory() {
    if (!_factory) {
        *_err<<"accessing dummy StreamFactory!\n";
        _factory = &create<StreamFactory>();
    }
    return *_factory;
}

StreamFactory & Environment::setStreamFactory(StreamFactory & factory) {
    StreamFactory *old = _factory;
    _factory = &factory;
    return *old;
}

MemoryAllocator & Environment::getAllocator() {
    return *ma;
}

MemoryAllocator & Environment::setAllocator(MemoryAllocator & allocator) {
    MemoryAllocator *old = ma;
    ma = &allocator;
    // refresh _memory_info from new allocator
    this->_memory_info = &ma->memInfo(this);
    return *old;
}
