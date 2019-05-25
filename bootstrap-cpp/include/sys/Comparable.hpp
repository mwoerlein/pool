#ifndef COMPARABLE_HPP_LOCK
#define COMPARABLE_HPP_LOCK

#include "sys/Object.hpp"

#define useComparableOperators(Obj) \
    using Comparable<Obj>::operator ==;\
    using Comparable<Obj>::operator !=;\
    using Comparable<Obj>::operator <;\
    using Comparable<Obj>::operator <=;\
    using Comparable<Obj>::operator >;\
    using Comparable<Obj>::operator >=;

#define useComparableCOperators(T) \
    using ComparableC<T>::operator ==;\
    using ComparableC<T>::operator !=;\
    using ComparableC<T>::operator <;\
    using ComparableC<T>::operator <=;\
    using ComparableC<T>::operator >;\
    using ComparableC<T>::operator >=;

template <class Obj> class Comparable: virtual public Object {
    public:
    virtual int compareTo(Obj &obj) = 0;
    
    inline bool operator ==(Obj &obj) { return compareTo(obj) == 0; }
    inline bool operator !=(Obj &obj) { return compareTo(obj) != 0; }
    inline bool operator < (Obj &obj) { return compareTo(obj) <  0; }
    inline bool operator <=(Obj &obj) { return compareTo(obj) <= 0; }
    inline bool operator > (Obj &obj) { return compareTo(obj) >  0; }
    inline bool operator >=(Obj &obj) { return compareTo(obj) >= 0; }

    virtual int hash() override {
        // has to be adjusted in final class to match equals
        return 0;
    }
    
    virtual bool equals(Object &o) override {
        if (Obj *obj = env().as<Obj>(o, rtti())) {
            return *this == *obj;
        }
        return false;
    }
};

template <typename T> class ComparableC {
    public:
    virtual int compareTo(T val) = 0;
    
    inline bool operator ==(T val) { return compareTo(val) == 0; }
    inline bool operator !=(T val) { return compareTo(val) != 0; }
    inline bool operator < (T val) { return compareTo(val) <  0; }
    inline bool operator <=(T val) { return compareTo(val) <= 0; }
    inline bool operator > (T val) { return compareTo(val) >  0; }
    inline bool operator >=(T val) { return compareTo(val) >= 0; }
};

#endif //COMPARABLE_HPP_LOCK

