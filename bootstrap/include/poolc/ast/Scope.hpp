#ifndef POOLC_AST_SCOPE_HPP_LOCK
#define POOLC_AST_SCOPE_HPP_LOCK

#include "sys/Object.hpp"

#include "sys/String.hpp"
#include "sys/collection/HashMap.hpp"

#include "poolc/ast/nodes/TranslationUnitNode.hpp"
#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"
#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"
#include "poolc/ast/nodes/instruction/BlockInstNode.hpp"

class UnitScope;
class ClassScope;
class InstanceScope;
class MethodScope;
class BlockScope;
class Scope: virtual public Object {
    protected:
    HashMap<String, ClassDeclNode> &classes;
    HashMap<String, MethodDeclNode> &methods;
    HashMap<String, VariableDeclNode> &variables;
    
    public:
    Scope * parent;
    
    Scope(Environment &env, MemoryInfo &mi, Scope *parent = 0);
    virtual ~Scope();
    
    virtual UnitScope * isUnit();
    virtual ClassScope * isClass();
    virtual InstanceScope * isInstance();
    virtual MethodScope * isMethod();
    virtual BlockScope * isBlock();
    
    virtual bool registerClass(ClassDeclNode & classDecl, String & alias);
    virtual ClassDeclNode * getClass(String & name);
    
    virtual bool registerMethod(MethodDeclNode & methodDecl);
    virtual MethodDeclNode * getMethod(String & name/*, MutableCollection<TypeRefNode> & parameters*/);
    
    virtual bool registerVariable(VariableDeclNode & variableDecl);
    virtual VariableDeclNode * getVariable(String & name);

    virtual TranslationUnitNode * getUnitNode();
    virtual ClassDeclNode * getClassDeclNode();
    virtual MethodDeclNode * getMethodDeclNode();
    virtual BlockInstNode * getBlockInstNode();
};

#endif //POOLC_AST_SCOPE_HPP_LOCK
