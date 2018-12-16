#ifndef POOLC_STORAGE_HEADERPARSER_HPP_LOCK
#define POOLC_STORAGE_HEADERPARSER_HPP_LOCK

#include "poolc/parser/ParseBuffer.hpp"

#include "sys/stream/IStream.hpp"
#include "sys/collection/PropertyContainer.hpp"

class HeaderParser: public ParseBuffer {
    protected:
    virtual void shift(size_t freed) override;
    
    public:
    HeaderParser(Environment &env, MemoryInfo &mi);
    virtual ~HeaderParser();
    
    virtual size_t parse(IStream & Input, PropertyContainer & result);
};

#endif //POOLC_STORAGE_HEADERPARSER_HPP_LOCK
