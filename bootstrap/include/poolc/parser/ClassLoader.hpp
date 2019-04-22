#ifndef POOLC_PARSER_CLASSLOADER_HPP_LOCK
#define POOLC_PARSER_CLASSLOADER_HPP_LOCK

#include "poolc/storage/ClassPathStorage.hpp"
#include "sys/collection/HashMap.hpp"

#include "sys/String.hpp"
#include "sys/log/LoggerAware.hpp"
#include "poolc/parser/PoolParser.hpp"
#include "poolc/ast/TypeManager.hpp"
#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"

class ClassResolver;
class PrettyPrinter;
class ClassLoader: public ClassPathStorage, virtual public LoggerAware, private HashMap<String, ClassDeclNode> {
    private:
    PoolParser & parser;
    ClassResolver & resolve;
    PrettyPrinter * pretty;
    
    public:
    ClassLoader(Environment &env, MemoryInfo &mi, TypeManager &types);
    virtual ~ClassLoader();
    
    virtual void setLogger(Logger &logger) override;
    virtual void setPrettyPrint(PrettyPrinter &pretty);
    
    virtual ClassDeclNode * getClass(String & fullQualifiedName);
    virtual void registerClass(ClassDeclNode & classDef);
    
    inline Iterator<String> & classNames() { return HashMap::keys(); }
};

#endif //POOLC_PARSER_CLASSLOADER_HPP_LOCK
