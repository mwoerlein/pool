#ifndef POOLC_AST_NODE_HPP_LOCK
#define POOLC_AST_NODE_HPP_LOCK

#include "sys/Object.hpp"

#include "poolc/ast/Visitor.hpp"

enum scope_t { scope_class, scope_instance };

class Node: virtual public Object {
    public:
    virtual ~Node();
    
    virtual bool accept(Visitor & visitor) = 0;
};

#endif //POOLC_AST_NODE_HPP_LOCK
