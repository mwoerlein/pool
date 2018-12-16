#ifndef POOLC_STORAGE_CLASSPATHSTORAGE_HPP_LOCK
#define POOLC_STORAGE_CLASSPATHSTORAGE_HPP_LOCK

#include "poolc/storage/PoolStorage.hpp"
#include "poolc/storage/StorageElement.hpp"

#include "sys/collection/LinkedList.hpp"

class ClassPathStorage: virtual public Object, protected LinkedList<PoolStorage> {
    protected:
    String & mimetypePool;
    
    public:
    ClassPathStorage(Environment & env, MemoryInfo & mi);
    virtual ~ClassPathStorage();
    
    virtual void addStore(PoolStorage & store);
    virtual StorageElement * getElement(String & classname);
};

#endif //POOLC_STORAGE_CLASSPATHSTORAGE_HPP_LOCK
