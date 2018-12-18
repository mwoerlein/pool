#ifndef POOLC_PARSER_CLASSLOADER_HPP_LOCK
#define POOLC_PARSER_CLASSLOADER_HPP_LOCK

#include "poolc/storage/ClassPathStorage.hpp"
#include "sys/collection/HashMap.hpp"

#include "sys/String.hpp"
#include "poolc/parser/PoolParser.hpp"
#include "poolc/ast/nodes/ClassDefNode.hpp"

class ResolveVisitor;
class ClassLoader: public ClassPathStorage, private HashMap<String, ClassDefNode> {
    private:
    PoolParser & parser;
    ResolveVisitor & resolve;
    
    public:
    ClassLoader(Environment &env, MemoryInfo &mi);
    virtual ~ClassLoader();
    
    virtual ClassDefNode * getClass(String & fullQualifiedName);
    virtual void registerClass(ClassDefNode & classDef);
};

#endif //POOLC_PARSER_CLASSLOADER_HPP_LOCK
