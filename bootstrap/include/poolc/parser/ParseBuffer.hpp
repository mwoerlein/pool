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
    
    int *linesBuffer;
    int *columnsBuffer;
    int bufferPos;
    int currentLine;
    int currentColumn;
        
    bool freeBuffer(size_t need);
    
    protected:
    virtual void shift(size_t freed) = 0;
    virtual void resetBuffer();
    virtual bool fillBuffer(size_t need, IStream & input);
    
    public:
    char *buffer;
    char *limit;
    char *token;
    char *current;
    char *marker;
    char *ctxmarker;
    
    public:
    ParseBuffer(Environment &env, MemoryInfo &mi, size_t size, size_t lookahead);
    virtual ~ParseBuffer();
    
    inline int getPos(char *ptr) { return bufferPos + (ptr - buffer); }
    inline int getLine(char *ptr) { return linesBuffer[ptr - buffer]; }
    inline int getColumn(char *ptr) { return columnsBuffer[ptr - buffer]; }
};

#endif //POOLC_PARSER_PARSEBUFFER_HPP_LOCK
