#include "poolc/ast/visitors/ResolveVisitor.hpp"

#include "poolc/ast/nodes/ClassRefNode.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
#include "poolc/ast/nodes/MethodRefNode.hpp"

// public
ResolveVisitor::ResolveVisitor(Environment &env, MemoryInfo &mi, SimpleFactory & factory)
        :Object(env, mi), factory(factory) {}
ResolveVisitor::~ResolveVisitor() {}

bool ResolveVisitor::visit(ClassRefNode & classRef) {
    if (!classRef.classDef) {
        classRef.classDef = factory.getDef(classRef.name);
        classRef.classDef->accept(*this);
    }
    return true;
}

bool ResolveVisitor::visit(ClassDefNode & classDef) {
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
            if (methodDef.naked) {
                continue;
            }
            MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(methodDef);
            MethodRefNode &old = classDef.methodRefs.set(methodDef.name, ref);
            if (&old) { old.destroy(); }
            methodDef.parent = ref.parent = &classDef;
        }
        mit.destroy();
    }
    return true;
}

bool ResolveVisitor::visit(MethodRefNode & methodRef) {
    return true;
}

bool ResolveVisitor::visit(MethodDefNode & methodDef) {
    return true;
}

bool ResolveVisitor::visit(VariableDefNode & variableDef) {
    return true;
}

bool ResolveVisitor::visit(CStringConstDefNode & constDef) {
    return true;
}

bool ResolveVisitor::visit(InlinePasmInstructionNode & pasmInstruction) {
    return true;
}
