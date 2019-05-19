#include "poolc/storage/ClassPathStorage.hpp"

#include "poolc/storage/Types.hpp"

// public
ClassPathStorage::ClassPathStorage(Environment & env, MemoryInfo & mi)
        :LinkedList(env, mi), Object(env, mi),
         mimetypePool(env.create<String, const char *>(MIMETYPE_POOL)),
         mimetypePasm(env.create<String, const char *>(MIMETYPE_PASM)) {
}

ClassPathStorage::~ClassPathStorage() {
    mimetypePool.destroy();
    mimetypePasm.destroy();
}

void ClassPathStorage::addStore(PoolStorage & store) {
    LinkedList<PoolStorage>::add(store);
}

StorageElement * ClassPathStorage::getElement(String & classname) {
    Iterator<PoolStorage> &it = LinkedList<PoolStorage>::iterator();
    StorageElement * result = 0;
    while (it.hasNext()) {
        result = it.next().getElement(classname, mimetypePool);
        if (result) {
            break;
        }
    }
    it.destroy();
    return result;
}

StorageElement * ClassPathStorage::getHeader(String & name) {
    String &search = env().create<String, String&>(name);
    search<<"_header";
    Iterator<PoolStorage> &it = LinkedList<PoolStorage>::iterator();
    StorageElement * result = 0;
    while (it.hasNext()) {
        result = it.next().getElement(search, mimetypePasm);
        if (result) {
            break;
        }
    }
    it.destroy();
    search.destroy();
    return result;
}

StorageElement * ClassPathStorage::getFooter(String & name) {
    String &search = env().create<String, String&>(name);
    search<<"_footer";
    Iterator<PoolStorage> &it = LinkedList<PoolStorage>::iterator();
    StorageElement * result = 0;
    while (it.hasNext()) {
        result = it.next().getElement(search, mimetypePasm);
        if (result) {
            break;
        }
    }
    it.destroy();
    search.destroy();
    return result;
}
