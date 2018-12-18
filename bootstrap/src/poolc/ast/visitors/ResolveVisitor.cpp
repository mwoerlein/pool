#include "poolc/ast/visitors/ResolveVisitor.hpp"

#include "poolc/ast/nodes/TranslationUnitNode.hpp"
#include "poolc/ast/nodes/NamespaceDefNode.hpp"
#include "poolc/ast/nodes/UseStatementNode.hpp"
#include "poolc/ast/nodes/ClassDefNode.hpp"
#include "poolc/ast/nodes/ClassRefNode.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
#include "poolc/ast/nodes/MethodRefNode.hpp"

// public
ResolveVisitor::ResolveVisitor(Environment &env, MemoryInfo &mi, ClassLoader & loader)
        :Object(env, mi), loader(loader) {}
ResolveVisitor::~ResolveVisitor() {}


bool ResolveVisitor::visit(TranslationUnitNode & translationUnit) {
    if (!translationUnit.ns) {
        env().err() << translationUnit.name << ": " << "missing namespace" << "\n";
        return false;
    }
    translationUnit.ns->accept(*this);
    translationUnit.uses.acceptAll(*this);
    translationUnit.classes.acceptAll(*this);
    return true;
}

bool ResolveVisitor::visit(NamespaceDefNode & namespaceDef) {
    return true;
}

bool ResolveVisitor::visit(UseStatementNode & useStmt) {
    return true;
}

bool ResolveVisitor::visit(ClassRefNode & classRef) {
    if (!classRef.classDef) {
        String &tmp = env().create<String>();
        // TODO: #6 get full qualified name from use-statements
        if (classRef.name == "A" || classRef.name == "B") {
            tmp << "my::" << classRef.name;
        } else {
            tmp << "my::core::" << classRef.name;
        }
        classRef.classDef = loader.getClass(tmp);
        tmp.destroy();
    }
    return true;
}

bool ResolveVisitor::visit(ClassDefNode & classDef) {
    if (!classDef.supers.isEmpty()) {
        env().out() << classDef.name << ": already resolved" << "\n";
    }
    
    if (!classDef.unit) {
        env().err() << classDef.name << ": " << "missing translation unit" << "\n";
    }
    classDef.fullQualifiedName << classDef.unit->ns->name << "::" << classDef.name;
    loader.registerClass(classDef);

    {
        Iterator<String> &it = classDef.fullQualifiedName.parts();
        while (it.hasNext()) {
            classDef.globalPrefix << '_' << it.next();
        }
        it.destroy();
        (classDef.localPrefix << '_').printuint(classDef.fullQualifiedName.hash(), 16, 8);
    }
    
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
        switch (methodDef.kind) {
            case naked:
                continue;
            case bootstrap:
                classDef.bootstrap = &methodDef;
        }
        MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(methodDef);
        MethodRefNode &old = classDef.methodRefs.set(methodDef.name, ref);
        if (&old) { old.destroy(); }
        methodDef.parent = ref.parent = &classDef;
    }
    mit.destroy();
    
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

bool ResolveVisitor::visit(IntConstDefNode & constDef) {
    return true;
}

bool ResolveVisitor::visit(InlinePasmInstructionNode & pasmInstruction) {
    return true;
}
