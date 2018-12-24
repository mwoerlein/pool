#ifndef POOLC_AST_NODES_INSTRUCTION_CSTRINGCONSTASSIGNNODE_HPP_LOCK
#define POOLC_AST_NODES_INSTRUCTION_CSTRINGCONSTASSIGNNODE_HPP_LOCK

#include "poolc/ast/Node.hpp"

class CStringConstAssignNode: public Node {
    public:
    String & name;
    String & value;
    
    CStringConstAssignNode(Environment &env, MemoryInfo &mi);
    virtual ~CStringConstAssignNode();
    
    virtual bool accept(Visitor & visitor) override;
};

#endif //POOLC_AST_NODES_INSTRUCTION_CSTRINGCONSTASSIGNNODE_HPP_LOCK
