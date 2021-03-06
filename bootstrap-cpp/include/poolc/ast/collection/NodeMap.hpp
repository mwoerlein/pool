#ifndef POOLC_AST_COLLECTION_NODEMAP_HPP_LOCK
#define POOLC_AST_COLLECTION_NODEMAP_HPP_LOCK

#include "sys/collection/HashMap.hpp"

#include "poolc/ast/Visitor.hpp"

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
        return true;
    }
    
    virtual void destroyAll() {
        Iterator<String> &it = this->keys();
        while (it.hasNext()) {
            String &key = it.next();
            this->get(key).destroy();
            key.destroy();
        }
        it.destroy();
        this->destroy();
    }
    
    virtual Obj * first() {
        Iterator<Obj> &it = this->iterator();
        Obj * ret = it.hasNext() ? &it.next() : 0;
        it.destroy();
        return ret;        
    }
};

#endif //POOLC_AST_COLLECTION_NODEMAP_HPP_LOCK
