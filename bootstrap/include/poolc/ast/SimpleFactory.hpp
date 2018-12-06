#ifndef POOLC_AST_SIMPLEFACTORY_HPP_LOCK
#define POOLC_AST_SIMPLEFACTORY_HPP_LOCK

#include "sys/Object.hpp"

#include "poolc/ast/nodes/ClassDefNode.hpp"

// TODO: #2 replace factory with parser
class SimpleFactory: virtual public Object {
    private:
    ClassDefNode * objectDef;
    ClassDefNode * classDef;
    ClassDefNode * threadDef;
    ClassDefNode * runtimeDef;
    ClassDefNode * aDef;
    ClassDefNode * bDef;
    
    virtual ClassDefNode & getObjectDef();
    virtual ClassDefNode & getClassDef();
    virtual ClassDefNode & getThreadDef();
    virtual ClassDefNode & getRuntimeDef();
    virtual ClassDefNode & getADef();
    virtual ClassDefNode & getBDef();
    
    public:
    SimpleFactory(Environment &env, MemoryInfo &mi);
    virtual ~SimpleFactory();
    virtual ClassDefNode * getDef(String &name);
};

#endif //POOLC_AST_SIMPLEFACTORY_HPP_LOCK
