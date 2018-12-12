#ifndef POOLC_PARSER_PARSEBUFFER_HPP_LOCK
#define POOLC_PARSER_PARSEBUFFER_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/stream/IStream.hpp"

#include "poolc/ast/nodes/ClassDefNode.hpp"

class ParseBuffer: virtual public Object {
    private:
    MemoryInfo & buffersInfo;
    size_t size;
    size_t lookahead;
        
    bool freeBuffer(size_t need);
    
    protected:
    virtual void shift(size_t freed) = 0;
    
    public:
    char *buffer;
    char *limit;
    char *token;
    char *current;
    char *marker;
    char *ctxmarker;
    
    int *linesBuffer;
    int *columnsBuffer;
    int currentLine;
    int currentColumn;
    
    public:
    ParseBuffer(Environment &env, MemoryInfo &mi, size_t size, size_t lookahead);
    virtual ~ParseBuffer();
    
    bool fillBuffer(size_t need, IStream & input);
};

#endif //POOLC_PARSER_PARSEBUFFER_HPP_LOCK
