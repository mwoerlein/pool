#ifndef POOLC_AST_SCOPE_HPP_LOCK
#define POOLC_AST_SCOPE_HPP_LOCK

#include "sys/Object.hpp"

#include "sys/String.hpp"
#include "sys/collection/HashMap.hpp"

class TranslationUnitNode;
class ClassDeclNode;
class MethodDeclNode;
class VariableDeclNode;
class BlockInstNode;
class MethodCallExprNode;

class UnitScope;
class ClassScope;
class InstanceScope;
class MethodScope;
class BlockScope;
class VariableScope;
class Scope: virtual public Object {
    protected:
    HashMap<String, ClassScope> &_classes;
    HashMap<String, MethodScope> &_methods;
    HashMap<String, VariableScope> &_variables;
    
    public:
    Scope * parent;
    
    Scope(Environment &env, MemoryInfo &mi, Scope *parent = 0);
    virtual ~Scope();
    
    virtual UnitScope * isUnit();
    virtual ClassScope * isClass();
    virtual InstanceScope * isInstance();
    virtual MethodScope * isMethod();
    virtual BlockScope * isBlock();
    virtual VariableScope * isVariable();
    
    virtual ClassScope * registerClass(ClassDeclNode & classDecl, String & alias);
    virtual ClassScope * registerClass(ClassScope & classScope);
    virtual ClassScope * registerClass(ClassScope & classScope, String & alias);
    virtual ClassScope * getClass(String & name);
    inline Iterator<ClassScope> &classes() { return _classes.values(); }
    
    virtual MethodScope * registerMethod(MethodDeclNode & methodDecl);
    virtual MethodScope * registerMethod(MethodScope & scope);
    virtual MethodScope * getMethod(String & name/*, MutableCollection<TypeRefNode> & parameters*/);
    virtual MethodScope * getMethod(MethodScope & scope);
    virtual MethodScope * getMethod(MethodCallExprNode & methodCall);
    inline Iterator<MethodScope> &methods() { return _methods.values(); }
    
    virtual VariableScope * registerVariable(VariableDeclNode & variableDecl);
    virtual VariableScope * registerVariable(VariableScope & scope);
    virtual VariableScope * getVariable(String & name);
    inline Iterator<VariableScope> &variables() { return _variables.values(); }

    virtual TranslationUnitNode * getUnitNode();
    virtual ClassDeclNode * getClassDeclNode();
    virtual MethodDeclNode * getMethodDeclNode();
    virtual BlockInstNode * getBlockInstNode();
    virtual VariableDeclNode * getVariableDeclNode();
};

#endif //POOLC_AST_SCOPE_HPP_LOCK
