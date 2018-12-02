#ifndef COLLECTION_MUTABLE_COLLECTION_HPP_LOCK
#define COLLECTION_MUTABLE_COLLECTION_HPP_LOCK

#include "sys/collection/Collection.hpp"

template <class Obj> class MutableCollection: virtual public Collection<Obj> {
    public:
    virtual bool add(Obj & o) = 0;
    virtual bool remove(Obj & o) = 0;
    virtual void clear() = 0;
};

#endif //COLLECTION_MUTABLE_COLLECTION_HPP_LOCK
