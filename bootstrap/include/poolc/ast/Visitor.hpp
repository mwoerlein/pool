#ifndef POOLC_AST_VISITOR_HPP_LOCK
#define POOLC_AST_VISITOR_HPP_LOCK

#include "sys/Object.hpp"

class ClassRefNode;
class ClassDefNode;
class MethodRefNode;
class MethodDefNode;
class VariableDefNode;
class CStringConstDefNode;
class Visitor: virtual public Object {
    public:
    virtual ~Visitor() {}
    
    virtual bool visit(ClassRefNode & classRef) = 0;
    virtual bool visit(ClassDefNode & classDef) = 0;
    virtual bool visit(MethodRefNode & methodRef) = 0;
    virtual bool visit(MethodDefNode & methodDef) = 0;
    virtual bool visit(VariableDefNode & variableDef) = 0;
    virtual bool visit(CStringConstDefNode & constDef) = 0;
};

#endif //POOLC_AST_VISITOR_HPP_LOCK

