#ifndef POOLC_AST_SCOPES_CLASSSCOPE_HPP_LOCK
#define POOLC_AST_SCOPES_CLASSSCOPE_HPP_LOCK

#include "poolc/ast/Scope.hpp"
#include "poolc/ast/Type.hpp"

#include "poolc/ast/collection/NodeMap.hpp"
#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"

class ClassScope: public Scope, public Type {
    private:
    ClassDeclNode & classDecl;
    HashMap<String, ClassScope> &_supers;
    HashMap<String, ClassScope> &_globalRequired;
    HashMap<String, String> &_strings;
    
    public:
    bool methodsRegistered;
    bool typesResolved;
    
    ClassScope(Environment &env, MemoryInfo &mi, Scope & parent, ClassDeclNode & classDecl);
    virtual ~ClassScope();
    virtual ClassScope * isClass() override;
    virtual ClassScope * getClass() override;
    virtual InstanceScope * getInstance() override;
    virtual ClassDeclNode * getClassDeclNode() override;
    
    virtual void addSuper(ClassScope &super);
    virtual bool hasSuper(ClassScope &super);
    inline ClassScope * firstSuper() { return _supers.first(); }
    inline Iterator<ClassScope> & supers() { return _supers.iterator(); }
    
    virtual void addGlobalRequired(ClassScope &required);
    inline Iterator<ClassScope> & globalRequired() { return _globalRequired.iterator(); }
    
    virtual String & stringId(String &string, String *id = 0);
    virtual Iterator<String> & strings();

    virtual OStream & operator >>(OStream & stream) override;
    virtual void printDebugName(OStream & stream) override;
};

#endif //POOLC_AST_SCOPES_CLASSSCOPE_HPP_LOCK
