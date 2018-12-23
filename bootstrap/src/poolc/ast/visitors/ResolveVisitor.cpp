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
        :Object(env, mi), LoggerAware(env, mi), loader(loader), curUnit(0) {}
ResolveVisitor::~ResolveVisitor() {}


bool ResolveVisitor::visit(TranslationUnitNode & translationUnit) {
    TranslationUnitNode *tmpUnit = curUnit;
    curUnit = &translationUnit;
    
    if (!curUnit->ns) {
        error() << curUnit->name << ": " << "missing namespace" << "\n";
        curUnit = tmpUnit;
        return false;
    }
    curUnit->ns->accept(*this);
    curUnit->uses.acceptAll(*this);
    curUnit->classes.acceptAll(*this);
    
    curUnit = tmpUnit;
    return true;
}

bool ResolveVisitor::visit(NamespaceDefNode & namespaceDef) {
    return true;
}

bool ResolveVisitor::visit(UseStatementNode & useStmt) {
    ClassDefNode *classDef = loader.getClass(useStmt.name);
    if (classDef) {
        curUnit->registerClass(useStmt.alias, *classDef);
    }
    return classDef;
}

bool ResolveVisitor::visit(ClassRefNode & classRef) {
    if (!classRef.classDef) {
        ClassDefNode * classDef = 0;
        if (classRef.name.isFullQualified()) {
            classRef.classDef = loader.getClass(classRef.name);
        } else {
            classRef.classDef = curUnit->getClass(classRef.name);
            if (!classRef.classDef) {
                String & fqn = env().create<String>();
                fqn << curUnit->ns->name << "::" << classRef.name;
                classRef.classDef = loader.getClass(fqn);
                fqn.destroy();
                if (classRef.classDef) {
                    curUnit->registerClass(classRef.classDef->name, *classRef.classDef);
                }
            }
        }
    }
    return classRef.classDef;
}

bool ResolveVisitor::visit(ClassDefNode & classDef) {
    if (!classDef.supers.isEmpty()) {
        info() << classDef.name << ": already resolved" << "\n";
    }
    
    classDef.fullQualifiedName << curUnit->ns->name << "::" << classDef.name;
    if (classDef.fullQualifiedName != curUnit->name) {
        error() << curUnit->name << ": class name '" << classDef.fullQualifiedName << "' does not match compilation unit\n";
    }
    classDef.unit = curUnit;
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
    {
        Iterator<MethodDefNode> &mit = classDef.methods.iterator();
        int index = 0;
        while (mit.hasNext()) {
            MethodDefNode &methodDef = mit.next();
            methodDef.parent = &classDef;
            methodDef.index = index++;
            switch (methodDef.kind) {
                case naked:
                    continue;
            }
            MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(methodDef);
            MethodRefNode &old = classDef.methodRefs.set(methodDef.name, ref);
            if (&old) { old.destroy(); }
            ref.parent = &classDef;
        }
        mit.destroy();
    }
    
    {
        Iterator<MethodRefNode> &mit = classDef.methodRefs.values();
        int index = 0;
        while (mit.hasNext()) {
            mit.next().index = index++;
        }
        mit.destroy();
    }
    
    if (curUnit->element.hasStringProperty("pool.bootstrap")) {
        String & bsName = curUnit->element.getStringProperty("pool.bootstrap");
        MethodRefNode &bsRef = classDef.methodRefs.get(bsName);
        if (!&bsRef) {
            warn() << curUnit->name << ": ignore missing bootstrap method '" << bsName << "'\n";
            curUnit->element.unsetProperty("pool.bootstrap");
            return false;
        }
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

bool ResolveVisitor::visit(IntConstDefNode & constDef) {
    return true;
}

bool ResolveVisitor::visit(InlinePasmInstructionNode & pasmInstruction) {
    return true;
}
