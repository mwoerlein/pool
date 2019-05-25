#include "sys/Object.hpp"

Object::Object(Environment &env, MemoryInfo &mi):_environment(env),_memory_info(&mi) {}
Object::~Object() {}

Environment & Object::env() {
    return _environment;
}

void Object::destroy() {
    _environment.destroy(*this);
}

int Object::hash() {
    return (int) _memory_info->buf;
}

RTTI Object::rtti() {
    return object;
}

bool Object::equals(Object &o) {
    return this->_memory_info->buf == o._memory_info->buf;
}

OStream & Object::operator >>(OStream & stream) {
    // TODO: prepend classname
    return stream << (void*) _memory_info->buf;
}
