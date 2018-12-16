#ifndef POOLC_STORAGE_POOLSTORAGE_HPP_LOCK
#define POOLC_STORAGE_POOLSTORAGE_HPP_LOCK

#include "sys/Object.hpp"

#include "sys/String.hpp"
#include "sys/stream/OStream.hpp"
#include "poolc/storage/StorageElement.hpp"

class PoolStorage: virtual public Object {
    public:
    virtual ~PoolStorage() {}
    
    virtual StorageElement * getElement(String & classname, String & mimetype) = 0;
    virtual OStream & writeElement(String & classname, String & mimetype) = 0;
};

#endif //POOLC_STORAGE_POOLSTORAGE_HPP_LOCK
