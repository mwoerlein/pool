#ifndef POOLC_AST_NODE_HPP_LOCK
#define POOLC_AST_NODE_HPP_LOCK

#define CLASSNAME_ID "CLASSNAME"

#include "sys/Object.hpp"

#include "poolc/ast/Visitor.hpp"
#include "sys/String.hpp"
#include "poolc/parser/token/FullQualifiedName.hpp"

class Scope;
class Node: virtual public Object {
    public:
    int first_line;
    int first_column;
    int last_line;
    int last_column;
    
    Scope * scope = 0;
    
    virtual ~Node();
    
    virtual bool accept(Visitor & visitor) = 0;
    
    virtual void printDebugName(OStream & stream) = 0;
    virtual void printDebugInfo(OStream & stream);
    virtual OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_AST_NODE_HPP_LOCK
