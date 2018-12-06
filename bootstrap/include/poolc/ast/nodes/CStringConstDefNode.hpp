#ifndef POOLC_AST_NODES_CSTRINGCONSTDEFNODE_HPP_LOCK
#define POOLC_AST_NODES_CSTRINGCONSTDEFNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

#include "sys/String.hpp"

class CStringConstDefNode: public Node {
    public:
    String & name;
    String & value;
    
    CStringConstDefNode(Environment &env, MemoryInfo &mi);
    virtual ~CStringConstDefNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_CSTRINGCONSTDEFNODE_HPP_LOCK
