#ifndef POOLC_STORAGE_STORAGEELEMENT_HPP_LOCK
#define POOLC_STORAGE_STORAGEELEMENT_HPP_LOCK

#include "sys/collection/PropertyContainer.hpp"
#include "sys/stream/SeekableIOStream.hpp"

class IOStreamStorageElement: public StorageElement {
    public:
    ClassPathStorage(Environment & env, MemoryInfo & mi, SeekableIOStream);
    virtual ~ClassPathStorage();
    virtual ~StorageElement() {}
    
    virtual SeekableIOStream & getContent() = 0;
};

#endif //POOLC_STORAGE_STORAGEELEMENT_HPP_LOCK
