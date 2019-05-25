#ifndef POOLC_STORAGE_OSTREAMPOOLSTORAGE_HPP_LOCK
#define POOLC_STORAGE_OSTREAMPOOLSTORAGE_HPP_LOCK

#include "poolc/storage/PoolStorage.hpp"
#include "poolc/storage/HeaderParser.hpp"

class OStreamPoolStorage: public PoolStorage {
    private:
    class OStreamPoolStorageOStream: public OStream {
        private:
        OStream &_out;
        public:
        OStreamPoolStorageOStream(Environment & env, MemoryInfo & mi, OStream & out): Object(env, mi), _out(out) {}
        virtual ~OStreamPoolStorageOStream() {}
        virtual OStream & operator <<(char c) override { _out<<c; }
    };
    OStream &out;
    
    public:
    OStreamPoolStorage(Environment & env, MemoryInfo & mi, OStream & out): Object(env, mi), out(out) {}
    virtual ~OStreamPoolStorage() {}
    
    virtual StorageElement * getElement(String & classname, String & mimetype) override { return 0; }
    virtual OStream & writeElement(String & classname, String & mimetype) override {
        return env().create<OStreamPoolStorageOStream, OStream&>(out);
    }
};

#endif //POOLC_STORAGE_OSTREAMPOOLSTORAGE_HPP_LOCK
