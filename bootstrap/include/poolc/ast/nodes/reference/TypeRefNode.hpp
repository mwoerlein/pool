#ifndef POOLC_AST_NODES_REFERENCE_TYPEREFNODE_HPP_LOCK
#define POOLC_AST_NODES_REFERENCE_TYPEREFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "poolc/ast/Type.hpp"

class TypeRefNode: virtual public Node {
    public:
    virtual ~TypeRefNode();
    
    virtual AllRefNode * isAll();
    virtual AnyRefNode * isAny();
    virtual ClassRefNode * isClass();
    virtual CStringRefNode * isCString();
    virtual IntRefNode * isInt();
};

#endif //POOLC_AST_NODES_REFERENCE_TYPEREFNODE_HPP_LOCK
