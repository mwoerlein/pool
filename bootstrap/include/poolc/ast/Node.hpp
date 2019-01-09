#ifndef POOLC_AST_NODE_HPP_LOCK
#define POOLC_AST_NODE_HPP_LOCK

#define CLASSNAME_ID "CLASSNAME"

#include "sys/Object.hpp"

#include "poolc/ast/Visitor.hpp"
#include "sys/String.hpp"
#include "poolc/parser/token/FullQualifiedName.hpp"

enum sign_t { sign_plus, sign_minus };
enum arith_op { op_add, op_sub, op_mul, op_div, op_mod };
enum arith_unary { unary_inc, unary_dec, unary_post_inc, unary_post_dec };

class Scope;
class Type;
class Node: virtual public Object {
    public:
    int first_line;
    int first_column;
    int last_line;
    int last_column;
    
    Scope * scope = 0;
    Type * resolvedType = 0;
    
    virtual ~Node();
    
    virtual bool accept(Visitor & visitor) = 0;
    
    virtual void printDebugName(OStream & stream) = 0;
    virtual void printDebugInfo(OStream & stream);
    virtual OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_AST_NODE_HPP_LOCK
