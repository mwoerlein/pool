#ifndef POOLC_PARSER_CLASSLOADER_HPP_LOCK
#define POOLC_PARSER_CLASSLOADER_HPP_LOCK

#include "poolc/storage/ClassPathStorage.hpp"
#include "sys/collection/HashMap.hpp"

#include "sys/String.hpp"
#include "sys/log/LoggerAware.hpp"
#include "poolc/parser/PoolParser.hpp"
#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"

class ResolveVisitor;
class ClassLoader: public ClassPathStorage, virtual public LoggerAware, private HashMap<String, ClassDeclNode> {
    private:
    PoolParser & parser;
    ResolveVisitor & resolve;
    
    public:
    ClassLoader(Environment &env, MemoryInfo &mi);
    virtual ~ClassLoader();
    
    virtual void setLogger(Logger &logger) override;
    
    virtual ClassDeclNode * getClass(String & fullQualifiedName);
    virtual void registerClass(ClassDeclNode & classDef);
};

#endif //POOLC_PARSER_CLASSLOADER_HPP_LOCK
