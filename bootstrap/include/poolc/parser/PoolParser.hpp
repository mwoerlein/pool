#ifndef POOLC_PARSER_POOLPARSER_HPP_LOCK
#define POOLC_PARSER_POOLPARSER_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/stream/IStream.hpp"

#include "poolc/ast/nodes/ClassDefNode.hpp"

class PoolParser: virtual public Object {
    private:
    ClassDefNode * cls;
    
    public:
    PoolParser(Environment &env, MemoryInfo &mi);
    virtual ~PoolParser();
    virtual ClassDefNode * parse(IStream &input);
    virtual bool error(const char * msg);
    virtual void registerClass(ClassDefNode *node) {
        cls = node;
    }
};

#endif //POOLC_PARSER_POOLPARSER_HPP_LOCK
