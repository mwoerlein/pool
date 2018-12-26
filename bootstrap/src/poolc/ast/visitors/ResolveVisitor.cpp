#include "poolc/ast/visitors/ResolveVisitor.hpp"

#include "poolc/ast/nodes/TranslationUnitNode.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"
#include "poolc/ast/nodes/declaration/NamespaceDeclNode.hpp"

#include "poolc/ast/nodes/reference/ClassRefNode.hpp"
#include "poolc/ast/nodes/reference/MethodRefNode.hpp"
#include "poolc/ast/nodes/reference/TypeRefNode.hpp"
#include "poolc/ast/nodes/reference/UseStatementNode.hpp"

#include "poolc/ast/scopes/UnitScope.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"

// public
ResolveVisitor::ResolveVisitor(Environment &env, MemoryInfo &mi, ClassLoader & loader)
        :Object(env, mi), LoggerAware(env, mi), loader(loader), curScope(0) {}
ResolveVisitor::~ResolveVisitor() {}


bool ResolveVisitor::visit(TranslationUnitNode & translationUnit) {
    Scope *tmpScope = curScope;
    translationUnit.scope = curScope = &env().create<UnitScope, TranslationUnitNode &>(translationUnit);
    
    if (!translationUnit.ns) {
        error() << translationUnit.name << ": " << "missing namespace" << "\n";
        curScope = tmpScope;
        return false;
    }
    translationUnit.ns->accept(*this);
    translationUnit.uses.acceptAll(*this);
    translationUnit.classes.acceptAll(*this);
    
    curScope = tmpScope;
    return true;
}

bool ResolveVisitor::visit(UseStatementNode & useStmt) {
    ClassDeclNode *classDef = loader.getClass(useStmt.name);
    if (classDef) {
        curScope->registerClass(*classDef, useStmt.alias);
    }
    return classDef;
}

bool ResolveVisitor::visit(ClassRefNode & classRef) {
    if (!classRef.classDef) {
        if (classRef.name.isFullQualified()) {
            classRef.classDef = loader.getClass(classRef.name);
        } else {
            classRef.classDef = curScope->getClass(classRef.name);
            if (!classRef.classDef) {
                String & fqn = env().create<String>();
                fqn << curScope->getUnitNode()->ns->name << "::" << classRef.name;
                classRef.classDef = loader.getClass(fqn);
                fqn.destroy();
                if (classRef.classDef) {
                    curScope->registerClass(*classRef.classDef, classRef.classDef->name);
                }
            }
        }
    }
    return classRef.classDef;
}

bool ResolveVisitor::visit(ClassDeclNode & classDef) {
    if (!classDef.supers.isEmpty()) {
        info() << classDef.name << ": already resolved" << "\n";
    }
    Scope *tmpScope = curScope;
    classDef.scope = curScope = &env().create<ClassScope, ClassDeclNode &, UnitScope &>(classDef, *tmpScope->isUnit());
    
    TranslationUnitNode * curUnit = curScope->getUnitNode();
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
    
    Iterator<TypeRefNode> &it = classDef.extends.iterator();
    while (it.hasNext()) {
        TypeRefNode & type = it.next();
        ClassRefNode *extend = type.isClass();
        if (!extend) {
            error() << curUnit->name << ": invalid extend type '" << type << "'\n";
        }
        extend->accept(*this);
        Iterator<ClassDeclNode> &sit = extend->classDef->supers.values();
        while (sit.hasNext()) {
            ClassDeclNode & super = sit.next();
            classDef.supers.set(super.fullQualifiedName, super);
        }
        sit.destroy();
        
        Iterator<MethodRefNode> &mit = extend->classDef->methodRefs.values();
        while (mit.hasNext()) {
            MethodRefNode &superRef = mit.next();
            MethodRefNode &ref = env().create<MethodRefNode, MethodDeclNode&>(superRef.methodDef);
            MethodRefNode &old = classDef.methodRefs.set(superRef.methodDef.name, ref);
            if (&old) { old.destroy(); }
            ref.parent = &classDef;
        }
        mit.destroy();
    }
    it.destroy();
    
    classDef.supers.set(classDef.fullQualifiedName, classDef);
    {
        Iterator<MethodDeclNode> &mit = classDef.methods.iterator();
        int index = 0;
        while (mit.hasNext()) {
            MethodDeclNode &methodDef = mit.next();
            methodDef.parent = &classDef;
            methodDef.index = index++;
            switch (methodDef.kind) {
                case naked:
                    continue;
            }
            MethodRefNode &ref = env().create<MethodRefNode, MethodDeclNode&>(methodDef);
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
            curScope = tmpScope;
            return false;
        }
    }
    curScope = tmpScope;
    return true;
}
