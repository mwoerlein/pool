#ifndef POOLC_AST_COLLECTION_NODELIST_HPP_LOCK
#define POOLC_AST_COLLECTION_NODELIST_HPP_LOCK

#include "sys/collection/LinkedList.hpp"

template <class Obj> class NodeList: public LinkedList<Obj> {
    public:
    NodeList(Environment &env, MemoryInfo &mi = *notAnInfo): LinkedList<Obj>(env, mi), Object(env, mi) {}
    virtual ~NodeList() {}
    
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
    
    virtual Obj * first() {
        Iterator<Obj> &it = this->iterator();
        Obj * ret = it.hasNext() ? &it.next() : 0;
        it.destroy();
        return ret;        
    }
};

#endif //POOLC_AST_COLLECTION_NODELIST_HPP_LOCK
