#ifndef POOLC_PARSER_POOLPARSER_HPP_LOCK
#define POOLC_PARSER_POOLPARSER_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/stream/IStream.hpp"

#include "poolc/ast/nodes/ClassDefNode.hpp"
#include "poolc/parser/ParseBuffer.hpp"

class Parser;
class PoolParser: public ParseBuffer {
    private:
    ClassDefNode * cls;
    IStream * input;

    int parseIntegerValue(char * start, char * end, int base);
    String * readString(char enclosure);
    String * readMultilineString();
    String * readLine();
    String * readComment();
    
    protected:
    virtual void shift(size_t freed) override;
    
    public:
    PoolParser(Environment &env, MemoryInfo &mi);
    virtual ~PoolParser();
    virtual ClassDefNode * parse(IStream &input);
    virtual bool error(const char * msg);
    virtual void registerClass(ClassDefNode *node) {
        cls = node;
    }
    virtual int lex(Parser & parser);
};

#endif //POOLC_PARSER_POOLPARSER_HPP_LOCK
