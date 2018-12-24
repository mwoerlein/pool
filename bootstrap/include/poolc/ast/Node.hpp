#ifndef POOLC_AST_NODE_HPP_LOCK
#define POOLC_AST_NODE_HPP_LOCK

#include "sys/Object.hpp"

#include "poolc/ast/Visitor.hpp"
#include "sys/String.hpp"
#include "poolc/parser/token/FullQualifiedName.hpp"

enum scope_t { scope_class, scope_instance, scope_method, scope_block };

class Node: virtual public Object {
    public:
    int first_line;
    int first_column;
    int last_line;
    int last_column;
    
    virtual ~Node();
    
    virtual bool accept(Visitor & visitor) = 0;
    
    virtual void printDebugName(OStream & stream) = 0;
    virtual void printDebugInfo(OStream & stream);
    virtual OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_AST_NODE_HPP_LOCK
