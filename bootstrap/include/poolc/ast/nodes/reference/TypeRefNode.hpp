#ifndef POOLC_AST_NODES_REFERENCE_TYPEREFNODE_HPP_LOCK
#define POOLC_AST_NODES_REFERENCE_TYPEREFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "poolc/ast/Type.hpp"

class TypeRefNode: public Node {
    public:
    Type * resolvedType = 0;
    virtual ~TypeRefNode();
    
    virtual ClassRefNode * isClass();
    virtual IntRefNode * isInt();
    virtual CStringRefNode * isCString();
};

#endif //POOLC_AST_NODES_REFERENCE_TYPEREFNODE_HPP_LOCK
