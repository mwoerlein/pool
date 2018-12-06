#ifndef POOLC_AST_COLLECTION_NODEMAP_HPP_LOCK
#define POOLC_AST_COLLECTION_NODEMAP_HPP_LOCK

#include "sys/String.hpp"
#include "sys/collection/HashMap.hpp"

template <class Obj> class NodeMap: public HashMap<String, Obj> {
    public:
    NodeMap(Environment &env, MemoryInfo &mi = *notAnInfo): HashMap<String, Obj>(env, mi), Object(env, mi) {}
    virtual ~NodeMap() {}
    
    virtual bool acceptAll(Visitor & visitor) {
        Iterator<Obj> &it = this->iterator();
        while (it.hasNext()) {
            it.next().accept(visitor);
        }
        it.destroy();
    }
    
    virtual void destroyAll() {
        Iterator<Obj> &it = this->iterator();
        while (it.hasNext()) {
            it.next().destroy();
        }
        it.destroy();
        this->destroy();
    }
};

#endif //POOLC_AST_COLLECTION_NODEMAP_HPP_LOCK
