#include "poolc/ast/visitors/Writer.hpp"

// public
Writer::Writer(Environment &env, MemoryInfo &mi, PoolStorage &ps, const char * mime)
        :Object(env, mi), LoggerAware(env, mi),
         mime(env.create<String, const char*>(mime)),
         ps(ps), element(0) {
}
Writer::~Writer() {
    finalizeElement();
    mime.destroy();
}

bool Writer::startElement(String & classname) {
    finalizeElement();
    element = &ps.writeElement(classname, mime);
    return element;
}

void Writer::finalizeElement() {
    if (element) {
        element->destroy();
        element = 0;
    }
}
