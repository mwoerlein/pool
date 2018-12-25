#ifndef POOLC_STORAGE_STORAGEELEMENT_HPP_LOCK
#define POOLC_STORAGE_STORAGEELEMENT_HPP_LOCK

#include "sys/collection/PropertyContainer.hpp"
#include "sys/stream/IStream.hpp"
#include "sys/stream/OStream.hpp"

class StorageElement: public PropertyContainer {
    public:
    StorageElement(Environment &env, MemoryInfo &mi): PropertyContainer(env, mi) {}
    virtual ~StorageElement() {}
    
    virtual IStream & getContent() = 0;
    virtual void dumpHeaders(OStream &out) = 0;
};

#endif //POOLC_STORAGE_STORAGEELEMENT_HPP_LOCK
