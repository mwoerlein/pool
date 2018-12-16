#ifndef COLLECTION_LINKEDLIST_HPP_LOCK
#define COLLECTION_LINKEDLIST_HPP_LOCK

#include "sys/collection/MutableCollection.hpp"

template <class Obj> class LinkedList: public MutableCollection<Obj> {
    private:
    class _Element: public Object {
        public:
        _Element *next;
        Obj *val;
        
        _Element(Environment &env, MemoryInfo &mi, Obj *val): Object(env, mi), next(0), val(val) {}
        virtual ~_Element() {}
    };
    class _Iterator: public Iterator<Obj> {
        public:
        _Element *_next;
        
        _Iterator(Environment &env, MemoryInfo &mi, _Element *next): Object(env, mi), _next(next) {}
        virtual ~_Iterator() {}
        virtual bool hasNext() {
            return _next != 0;
        }
        virtual Obj & next() {
            if (!_next) {
                return *(Obj *)0;
            }
            Obj *val = _next->val;
            _next = _next->next;
            return *val;
        }
    };
    
    _Element *_first;
    _Element *_last;
    int _size;
    
    public:
    LinkedList(Environment &env, MemoryInfo &mi = *notAnInfo): Object(env, mi), _first(0), _last(0), _size(0) {}
    virtual ~LinkedList() {
        clear();
    }
    
    virtual void clear() override {
        _Element * cur = _first;
        while (cur) {
            _Element * kill = cur;
            cur = cur->next;
            kill->destroy();
        }
        _last = _first = 0;
        _size = 0;
    }
    
    virtual int size() override {
        return _size;
    }
    
    virtual bool add(Obj & o) override {
        _Element *newElem = &Object::env().create<_Element, Obj*>(&o);
        if (_size) {
            _last = _last->next = newElem;
        } else {
            _first = _last = newElem;
        }
        _size++;
        return true;
    }
    
    virtual bool contains(Obj & o) override {
        for (_Element *e = _first; e; e = e->next) {
            if (e->val->equals(o)) {
                return true;
            }
        }
        return false;
    }
    
    virtual bool remove(Obj & o) override {
        if (_size) {
            if (_first->val->equals(o)) {
                _Element *found = _first;
                _first = found->next;
                if (_last == found) {
                    _last = 0;
                }
                --_size;
                found->destroy();
                return true;
            }
            
            for (_Element *e = _first; e->next; e = e->next) {
                if (e->next->val->equals(o)) {
                    _Element *found = e->next;
                    e->next = found->next;
                    if (_last == found) {
                        _last = e;
                    }
                    --_size;
                    found->destroy();
                    return true;
                }
            }
        }        
        return false;
    }
    
    virtual Iterator<Obj> & iterator() override {
        return Object::env().create<_Iterator, _Element*>(_first);
    }
    
    virtual Obj * first() { return _first ? _first->val : 0; }
    virtual Obj * last() { return _last ? _last->val : 0; }
};

#endif //COLLECTION_LINKEDLIST_HPP_LOCK
