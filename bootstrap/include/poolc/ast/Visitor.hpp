#ifndef POOLC_AST_VISITOR_HPP_LOCK
#define POOLC_AST_VISITOR_HPP_LOCK

#include "sys/Object.hpp"

class ClassDefNode;
class MethodDefNode;
class VariableDefNode;
class Visitor: virtual public Object {
    public:
    virtual ~Visitor() {}
    
    virtual bool visit(ClassDefNode & classDef) = 0;
    virtual bool visit(MethodDefNode & methodDef) = 0;
    virtual bool visit(VariableDefNode & variableDef) = 0;
};

#endif //POOLC_AST_VISITOR_HPP_LOCK

