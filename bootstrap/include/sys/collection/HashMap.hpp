#ifndef COLLECTION_HASHMAP_HPP_LOCK
#define COLLECTION_HASHMAP_HPP_LOCK

#include "sys/collection/MutableMap.hpp"

template <class Key, class Value> class HashMap: public MutableMap<Key,Value> {
    private:
    class _Element: public Object {
        public:
        _Element *orderedPrev;
        _Element *orderedNext;
        _Element *bucketNext;
        Key *key;
        Value *value;
        
        _Element(Environment &env, MemoryInfo &mi, Key *key, Value *value): Object(env, mi), key(key), value(value), orderedPrev(0), orderedNext(0), bucketNext(0) {}
        virtual ~_Element() {}
    };
    class _KeyIterator: public Iterator<Key> {
        public:
        _Element *_next;
        
        _KeyIterator(Environment &env, MemoryInfo &mi, _Element *next): Object(env, mi), _next(next) {}
        virtual ~_KeyIterator() {}
        virtual bool hasNext() {
            return _next != 0;
        }
        virtual Key & next() {
            if (!_next) {
                return *(Key *)0;
            }
            Key *key = _next->key;
            _next = _next->orderedNext;
            return *key;
        }
    };
    class _ValueIterator: public Iterator<Value> {
        public:
        _Element *_next;
        
        _ValueIterator(Environment &env, MemoryInfo &mi, _Element *next): Object(env, mi), _next(next) {}
        virtual ~_ValueIterator() {}
        virtual bool hasNext() {
            return _next != 0;
        }
        virtual Value & next() {
            if (!_next) {
                return *(Value *)0;
            }
            Value *value = _next->value;
            _next = _next->orderedNext;
            return *value;
        }
    };

    MemoryInfo * bucketsInfo;
    _Element *first, *last, **buckets;
    int _size;
    int bucketsCount;
    int initialBucketsCount;
    
    void resize(int count) {
        bucketsCount = count;
        Object::env().getAllocator().free(*bucketsInfo);
        bucketsInfo = &Object::env().getAllocator().allocate(bucketsCount * sizeof(_Element*));
        buckets = (_Element**) bucketsInfo->buf;
    }
    
    void rehash() {
        for (int i = 0; i < bucketsCount; i++) {
            buckets[i] = 0;
        }
        for (_Element * cur = first; cur; cur = cur->orderedNext) {
            int bucketNumber = getBucketNumber(*cur->key);
            cur->bucketNext = buckets[bucketNumber];
            buckets[bucketNumber] = cur;
        }
    }
    
    void destroyAllElements() {
        _Element * cur = first;
        while (cur) {
            _Element * kill = cur;
            cur = cur->orderedNext;
            kill->destroy();
        }
    }
    
    protected:
    inline int getBucketNumber(Key &k) {
        return ((unsigned int)k.hash()) % bucketsCount;
    }
    
    virtual _Element * findInBucket(_Element *bucket, Key &k) {
        while (bucket) {
            if (bucket->key->equals(k)) {
                return bucket;
            }
            bucket = bucket->bucketNext;
        }
        return 0;
    }

    public:
    HashMap(Environment &env, MemoryInfo &mi = *notAnInfo, int bucketsCount = 13):
            Object(env, mi), bucketsCount(bucketsCount), initialBucketsCount(bucketsCount),
            bucketsInfo(&env.getAllocator().allocate(bucketsCount * sizeof(_Element*))),
            buckets((_Element**) bucketsInfo->buf),
            first(0), last(0), _size(0) {
        rehash();
    }
    virtual ~HashMap() {
        destroyAllElements();
        Object::env().getAllocator().free(*bucketsInfo);
    }
    
    virtual void clear() override {
        destroyAllElements();
        last = first = 0;
        _size = 0;
        // shrink, if required
        if (bucketsCount != initialBucketsCount) {
            resize(initialBucketsCount);
        }
        // clear buckets
        rehash();
    }
    
    virtual int size() override {
        return _size;
    }
    
    virtual Value & set(Key & k, Value & v) override {
        // grow, if required
        if ((2 * _size) > bucketsCount) {
            resize(2*bucketsCount + 1);
            rehash();
        }
        int bucketNumber = getBucketNumber(k);
        if (_Element *e = findInBucket(buckets[bucketNumber], k)) {
            // replace value
            Value *ret = e->value;
            e->value = &v;
            return *ret;
        }
        
        _Element *newElem = &Object::env().create<_Element, Key*, Value*>(&k, &v);
        if (_size) {
            newElem->orderedPrev = last;
            last->orderedNext = newElem;
            last = newElem;
        } else {
            first = last = newElem;
        }
        newElem->bucketNext = buckets[bucketNumber];
        buckets[bucketNumber] = newElem;
        _size++;
        return *(Value*) 0;
    }
    
    virtual Value & unset(Key & k) override {
        int bucketNumber = getBucketNumber(k);
        _Element *e = buckets[bucketNumber];
        
        // search and remove from bucket list
        if (e) {
            if (e->key->equals(k)) {
                // found as first of bucket list
                buckets[bucketNumber] = e->bucketNext;
            } else do {
                _Element *prev = e;
                e = e->bucketNext;
                if (e->key->equals(k)) {
                    // found in bucket list
                    prev->bucketNext = e->bucketNext;
                    break;
                }
            } while (e);
        }
        
        // remove from ordered list
        if (e) {
            Value *ret = e->value;
            if (e->orderedPrev) {
                e->orderedPrev->orderedNext = e->orderedNext;
            } else { // first element
                first = e->orderedNext;
            }
            if (e->orderedNext) {
                e->orderedNext->orderedPrev = e->orderedPrev;
            } else { // last element
                last = e->orderedPrev;
            }
            e->orderedPrev = e->orderedNext = 0; 
            e->destroy();
            _size--;
            
            // shrink, if required
            if (((4 * _size) < bucketsCount) && bucketsCount > initialBucketsCount) {
                resize((bucketsCount-1) / 2);
                rehash();
            }
        
            return *ret;
        }
        return *(Value*) 0;
    }
    
    virtual Value & get(Key & k) override {
        if (_Element *e = findInBucket(buckets[getBucketNumber(k)], k)) {
            return *e->value;
        }
        return *(Value*) 0;
    }
    
    virtual bool has(Key & k) override {
        if (_Element *e = findInBucket(buckets[getBucketNumber(k)], k)) {
            return true;
        }
        return false;
    }
    
    virtual bool contains(Value & v) override {
        for (_Element *e = first; e; e = e->orderedNext) {
            if (e->value->equals(v)) {
                return true;
            }
        }
        return false;
    }
    
    virtual Iterator<Key> & keys() override {
        return Object::env().create<_KeyIterator, _Element*>(first);
    }
    
    virtual Iterator<Value> & values() override {
        return Object::env().create<_ValueIterator, _Element*>(first);
    }
    
    virtual void dumpBuckets(OStream &out) {
        for (int i = 0; i < bucketsCount; i++) {
            int size = 0;
            for (_Element *e = buckets[i]; e; e = e->bucketNext) { size++; }
            out<<"bucket "<<i<<": size="<<size<<"\n";
        }
    }
};

#endif //COLLECTION_HASHMAP_HPP_LOCK
