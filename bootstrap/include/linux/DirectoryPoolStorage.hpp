#ifndef LINUX_DIRECTORYPOOLSTORAGE_HPP_LOCK
#define LINUX_DIRECTORYPOOLSTORAGE_HPP_LOCK

#include "poolc/storage/PoolStorage.hpp"
#include "poolc/storage/HeaderParser.hpp"

class DirectoryPoolStorage: public PoolStorage {
    private:
    const char * baseDir;
    HeaderParser & headerParser;
    
    String & buildFilePath(String & classname, String & mimetype, bool createPath = false);
    
    public:
    DirectoryPoolStorage(Environment & env, MemoryInfo & mi, const char * baseDir = ".");
    DirectoryPoolStorage(Environment & env, MemoryInfo & mi, String & baseDir);
    virtual ~DirectoryPoolStorage();
    
    virtual StorageElement * getElement(String & classname, String & mimetype) override;
    virtual OStream & writeElement(String & classname, String & mimetype) override;
    
    virtual String &getTemporaryClassname(const char * prefix = "pool");
    virtual void removeElement(String & classname, String & mimetype);
};

#endif //LINUX_DIRECTORYPOOLSTORAGE_HPP_LOCK
