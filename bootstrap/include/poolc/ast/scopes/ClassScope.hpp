#ifndef POOLC_AST_SCOPES_CLASSSCOPE_HPP_LOCK
#define POOLC_AST_SCOPES_CLASSSCOPE_HPP_LOCK

#include "poolc/ast/Scope.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/scopes/UnitScope.hpp"

class ClassScope: public Scope {
    private:
    ClassDeclNode & classDecl;
    
    public:
    ClassScope(Environment &env, MemoryInfo &mi, ClassDeclNode & classDecl, UnitScope & parent);
    virtual ~ClassScope();
    virtual ClassScope * isClass() override;
    virtual ClassDeclNode * getClassDeclNode() override;
};

#endif //POOLC_AST_SCOPES_CLASSSCOPE_HPP_LOCK
