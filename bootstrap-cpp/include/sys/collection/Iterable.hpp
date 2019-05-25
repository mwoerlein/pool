#ifndef COLLECTION_ITERABLE_HPP_LOCK
#define COLLECTION_ITERABLE_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/collection/Iterator.hpp"

template <class Obj> class Iterable: virtual public Object {
    public:
    virtual Iterator<Obj> & iterator() = 0;
};

#endif //COLLECTION_ITERABLE_HPP_LOCK
