#ifndef COLLECTION_MUTABLE_MAP_HPP_LOCK
#define COLLECTION_MUTABLE_MAP_HPP_LOCK

#include "sys/collection/Map.hpp"

template <class Key, class Value> class MutableMap: virtual public Map<Key, Value> {
    public:
    virtual Value & set(Key & k, Value & v) = 0;
    virtual Value & unset(Key & k) = 0;
    virtual void clear() = 0;
};

#endif //COLLECTION_MUTABLE_MAP_HPP_LOCK
