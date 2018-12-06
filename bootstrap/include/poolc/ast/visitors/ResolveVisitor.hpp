#ifndef POOLC_AST_VISITORS_RESOLVEVISITOR_HPP_LOCK
#define POOLC_AST_VISITORS_RESOLVEVISITOR_HPP_LOCK

#include "poolc/ast/Visitor.hpp"
#include "poolc/ast/SimpleFactory.hpp"

class ResolveVisitor: public Visitor {
    private:
    SimpleFactory & factory;
    
    public:
    ResolveVisitor(Environment &env, MemoryInfo &mi, SimpleFactory & factory)
            :Object(env, mi), factory(factory) {}
    virtual ~ResolveVisitor() {}
    
    virtual bool visit(ClassRefNode & classRef) {
        if (!classRef.classDef) {
            classRef.classDef = factory.getDef(classRef.name);
            classRef.classDef->accept(*this);
        }
        return true;
    }
    
    virtual bool visit(ClassDefNode & classDef) {
        if (classDef.supers.isEmpty()) {
            Iterator<ClassRefNode> &it = classDef.extends.iterator();
            while (it.hasNext()) {
                ClassRefNode & extend = it.next();
                extend.accept(*this);
                Iterator<ClassDefNode> &sit = extend.classDef->supers.values();
                while (sit.hasNext()) {
                    ClassDefNode & super = sit.next();
                    classDef.supers.set(super.fullQualifiedName, super);
                }
                sit.destroy();
                
                Iterator<MethodRefNode> &mit = extend.classDef->methodRefs.values();
                while (mit.hasNext()) {
                    MethodRefNode &superRef = mit.next();
                    MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(superRef.methodDef);
                    MethodRefNode &old = classDef.methodRefs.set(superRef.methodDef.name, ref);
                    if (&old) { old.destroy(); }
                    ref.parent = &classDef;
                }
                mit.destroy();
            }
            it.destroy();
            
            classDef.supers.set(classDef.fullQualifiedName, classDef);
          
            Iterator<MethodDefNode> &mit = classDef.methods.iterator();
            while (mit.hasNext()) {
                MethodDefNode &methodDef = mit.next();
                MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(methodDef);
                MethodRefNode &old = classDef.methodRefs.set(methodDef.name, ref);
                if (&old) { old.destroy(); }
                methodDef.parent = ref.parent = &classDef;
            }
            mit.destroy();
        }
        return true;
    }
    
    virtual bool visit(MethodRefNode & methodRef) {
        return true;
    }
    
    virtual bool visit(MethodDefNode & methodDef) {
        return true;
    }
    
    virtual bool visit(VariableDefNode & variableDef) {
        return true;
    }
    
    virtual bool visit(CStringConstDefNode & constDef) {
        return true;
    }
    
    virtual bool visit(InlinePasmInstructionNode & pasmInstruction) {
        return true;
    }
};

#endif //POOLC_AST_VISITORS_RESOLVEVISITOR_HPP_LOCK

