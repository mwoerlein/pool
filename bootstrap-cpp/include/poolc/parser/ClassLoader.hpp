#ifndef POOLC_PARSER_CLASSLOADER_HPP_LOCK
#define POOLC_PARSER_CLASSLOADER_HPP_LOCK

#include "poolc/storage/ClassPathStorage.hpp"
#include "sys/collection/HashMap.hpp"

#include "sys/String.hpp"
#include "sys/log/LoggerAware.hpp"
#include "poolc/parser/PoolParser.hpp"
#include "poolc/ast/TypeManager.hpp"
#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/declaration/StructDeclNode.hpp"
#include "poolc/ast/scopes/NamedType.hpp"

class ClassResolver;
class PrettyPrinter;
class ClassLoader: virtual public LoggerAware, private HashMap<String, ClassDeclNode> {
    private:
    ClassPathStorage & classPath;
    PoolParser & parser;
    ClassResolver & resolve;
    PrettyPrinter * pretty;
    HashMap<String, ClassDeclNode> &classes;
    HashMap<String, StructDeclNode> &structs;
    
    bool initialize(String & fullQualifiedName);
    
    public:
    ClassLoader(Environment &env, MemoryInfo &mi, ClassPathStorage &classPath, TypeManager &types);
    virtual ~ClassLoader();
    
    virtual void setLogger(Logger &logger) override;
    virtual void setPrettyPrint(PrettyPrinter &pretty);
    
    virtual ClassDeclNode * getClass(String & fullQualifiedName); // deprecated?
    virtual void registerClass(ClassDeclNode & classDecl);
    inline Iterator<String> & classNames() { return classes.keys(); }
    
    virtual NamedType * getNamedType(String & fullQualifiedName);
    virtual void registerStruct(StructDeclNode & structDecl);
    
};

#endif //POOLC_PARSER_CLASSLOADER_HPP_LOCK
