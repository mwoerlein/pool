#ifndef POOLC_AST_PARSER_TOKEN_FULLQUALIFIEDNAME_HPP_LOCK
#define POOLC_AST_PARSER_TOKEN_FULLQUALIFIEDNAME_HPP_LOCK

#include "sys/String.hpp"
#include "sys/collection/LinkedList.hpp"

class FullQualifiedName: public String {
    protected:
    LinkedList<String> * _parts;
    
    virtual LinkedList<String> & getParts();
    virtual void markModified() override;
    
    public:
    FullQualifiedName(Environment &env, MemoryInfo &mi);
    virtual ~FullQualifiedName();

    using String::operator =;
    using OStream::operator <<;
    
    inline Iterator<String> & parts() { return getParts().iterator(); }
    inline String & lastPart() { return *getParts().last(); }
};

#endif //POOLC_AST_PARSER_TOKEN_FULLQUALIFIEDNAME_HPP_LOCK
