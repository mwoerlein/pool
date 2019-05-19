#ifndef POOLC_STORAGE_CLASSPATHSTORAGE_HPP_LOCK
#define POOLC_STORAGE_CLASSPATHSTORAGE_HPP_LOCK

#include "poolc/storage/PoolStorage.hpp"
#include "poolc/storage/StorageElement.hpp"

#include "sys/collection/LinkedList.hpp"

class ClassPathStorage: virtual public Object, protected LinkedList<PoolStorage> {
    protected:
    String & mimetypePool;
    String & mimetypePasm;
    
    public:
    ClassPathStorage(Environment & env, MemoryInfo & mi);
    virtual ~ClassPathStorage();
    
    virtual void addStore(PoolStorage & store);
    virtual StorageElement * getElement(String & classname);
    virtual StorageElement * getHeader(String & name);
    virtual StorageElement * getFooter(String & name);
};

#endif //POOLC_STORAGE_CLASSPATHSTORAGE_HPP_LOCK
