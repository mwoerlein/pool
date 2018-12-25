#ifndef POOLC_AST_VISITORS_WRITER_HPP_LOCK
#define POOLC_AST_VISITORS_WRITER_HPP_LOCK

#include "poolc/ast/Visitor.hpp"

#include "sys/String.hpp"
#include "sys/stream/OStream.hpp"
#include "poolc/storage/PoolStorage.hpp"

class Writer: public Visitor {
    protected:
    String & mime;
    PoolStorage & ps;
    OStream * element;
    
    public:
    Writer(Environment &env, MemoryInfo &mi, PoolStorage &ps, const char * mime);
    virtual ~Writer();
    
    inline OStream & elem() { return *element; }
    
    virtual bool startElement(String & classname);
    virtual void finalizeElement();
};

#endif //POOLC_AST_VISITORS_WRITER_HPP_LOCK
