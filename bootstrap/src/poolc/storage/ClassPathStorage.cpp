#include "poolc/storage/ClassPathStorage.hpp"

#include "poolc/storage/Types.hpp"

// public
ClassPathStorage::ClassPathStorage(Environment & env, MemoryInfo & mi)
        :LinkedList(env, mi), Object(env, mi),
         mimetypePool(env.create<String, const char *>(MIMETYPE_POOL)) {
}

ClassPathStorage::~ClassPathStorage() {
    mimetypePool.destroy();
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
