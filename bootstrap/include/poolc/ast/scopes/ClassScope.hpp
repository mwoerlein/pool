#ifndef POOLC_AST_SCOPES_CLASSSCOPE_HPP_LOCK
#define POOLC_AST_SCOPES_CLASSSCOPE_HPP_LOCK

#include "poolc/ast/Scope.hpp"

#include "poolc/ast/collection/NodeMap.hpp"
#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"

class ClassScope: public Scope {
    private:
    ClassDeclNode & classDecl;
    HashMap<String, ClassScope> &_supers;
    
    public:
    
    ClassScope(Environment &env, MemoryInfo &mi, Scope & parent, ClassDeclNode & classDecl);
    virtual ~ClassScope();
    virtual ClassScope * isClass() override;
    virtual ClassDeclNode * getClassDeclNode() override;
    
    virtual void addSuper(ClassScope &super);
    inline ClassScope * firstSuper() { return _supers.first(); }
    inline Iterator<ClassScope> & supers() { return _supers.iterator(); }
};

#endif //POOLC_AST_SCOPES_CLASSSCOPE_HPP_LOCK
