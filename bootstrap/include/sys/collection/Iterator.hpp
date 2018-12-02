#ifndef COLLECTION_ITERATOR_HPP_LOCK
#define COLLECTION_ITERATOR_HPP_LOCK

#include "sys/Object.hpp"

template <class Obj> class Iterator: virtual public Object {
    public:
    virtual bool hasNext() = 0;
    virtual Obj & next() = 0;
};

#endif //COLLECTION_ITERATOR_HPP_LOCK
