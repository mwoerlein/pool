#ifndef COLLECTION_MAP_HPP_LOCK
#define COLLECTION_MAP_HPP_LOCK

#include "sys/collection/Collection.hpp"

template <class Key, class Value> class Map: virtual public Collection<Value> {
    public:
    virtual bool has(Key & k) = 0;
    virtual Value & get(Key & k) = 0;
    virtual Iterator<Key> & keys() = 0;
    virtual Iterator<Value> & values() = 0;
    virtual Iterator<Value> & iterator() override { return values(); }
};

#endif //COLLECTION_MAP_HPP_LOCK
