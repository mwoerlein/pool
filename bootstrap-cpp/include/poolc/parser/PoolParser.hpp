#ifndef POOLC_PARSER_POOLPARSER_HPP_LOCK
#define POOLC_PARSER_POOLPARSER_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/log/LoggerAware.hpp"
#include "sys/re2c/ParseBuffer.hpp"
#include "sys/stream/IStream.hpp"

#include "poolc/ast/nodes/TranslationUnitNode.hpp"
#include "poolc/storage/StorageElement.hpp"

class Parser;
class PoolParser: public ParseBuffer, virtual public LoggerAware {
    private:
    TranslationUnitNode * unit;
    IStream * input;
    Parser * parser;

    int parseIntegerValue(char * start, char * end, int base);
    String * readString(char enclosure);
    String * readMultilineString();
    String * readLine();
    String * readComment();
    
    protected:
    virtual void shift(size_t freed) override;
    virtual void setLocation();
    
    public:
    PoolParser(Environment &env, MemoryInfo &mi);
    virtual ~PoolParser();
    virtual TranslationUnitNode * parse(StorageElement &element, String &name);
    
    virtual TranslationUnitNode * getUnit();
    virtual bool logError(const char * msg, int line = -1, int column = -1);
    virtual int lex();
};

#endif //POOLC_PARSER_POOLPARSER_HPP_LOCK
