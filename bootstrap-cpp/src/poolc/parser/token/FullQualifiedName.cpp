#include "poolc/parser/token/FullQualifiedName.hpp"

// public
FullQualifiedName::FullQualifiedName(Environment &env, MemoryInfo &mi):
    String(env, mi),
    Object(env, mi),
    _parts(0) {
}

FullQualifiedName::~FullQualifiedName() {
    markModified();
}

LinkedList<String> & FullQualifiedName::getParts() {
    if (!_parts) {
        _parts = &env().create<LinkedList<String>>();
        String & tmp = env().create<String>();
        IStream &in = toIStream();
        char c;
        while (!in.isEmpty()) {
            in >> c;
            if (c == ':') {
                if (tmp.length()) {
                    _parts->add(env().create<String, String&>(tmp));
                }
                tmp = "";
            } else {
                tmp << c;
            }
        }
        if (tmp.length()) {
            _parts->add(env().create<String, String&>(tmp));
        }
        in.destroy();
        tmp.destroy();
    }
    return *_parts;
}

void FullQualifiedName::markModified() {
    if (_parts) {
        Iterator<String> &it = _parts->iterator();
        while (it.hasNext()) {
            it.next().destroy();
        }
        it.destroy();
        _parts->destroy();
        _parts = 0;
    }
}
