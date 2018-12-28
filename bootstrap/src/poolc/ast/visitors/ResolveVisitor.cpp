#include "poolc/ast/visitors/ResolveVisitor.hpp"

#include "poolc/ast/nodes/all.hpp"

#include "poolc/ast/scopes/UnitScope.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"
#include "poolc/ast/scopes/BlockScope.hpp"
#include "poolc/ast/scopes/VariableScope.hpp"

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
    useStmt.scope = curScope;
    ClassDeclNode *classDecl = loader.getClass(useStmt.name);
    if (classDecl && classDecl->scope && classDecl->scope->isClass()) {
        useStmt.resolvedType = curScope->registerClass(*classDecl->scope->isClass(), useStmt.alias);
    } else {
        warn() << "unresolved type '" << useStmt.name << "' loaded!\n";
    }
    return useStmt.resolvedType;
}

bool ResolveVisitor::visit(ClassRefNode & classRef) {
    if (!classRef.resolvedType) {
        if (classRef.name.isFullQualified()) {
            ClassDeclNode *classDecl = loader.getClass(classRef.name);
            if (classDecl && classDecl->scope && classDecl->scope->isClass()) {
                classRef.resolvedType = classDecl->scope->isClass();
            } else {
                warn() << "unresolved type '" << classRef.name << "' loaded!\n";
            }
        } else {
            classRef.resolvedType = curScope->getClass(classRef.name);
            if (!classRef.resolvedType) {
                String & fqn = env().create<String>();
                fqn << curScope->getUnitNode()->ns->name << "::" << classRef.name;
                ClassDeclNode *classDecl = loader.getClass(fqn);
                if (classDecl && classDecl->scope && classDecl->scope->isClass()) {
                    classRef.resolvedType = curScope->registerClass(*classDecl->scope->isClass(), classRef.name);
                } else {
                    warn() << "unresolved type '" << fqn << "' loaded!\n";
                }
                fqn.destroy();
            }
        }
    }
    return classRef.resolvedType;
}

bool ResolveVisitor::visit(ClassDeclNode & classDecl) {
    if (classDecl.scope) {
        info() << classDecl.name << ": already resolved" << "\n";
        return false;
    }
    
    TranslationUnitNode * curUnit = curScope->getUnitNode();
    classDecl.fullQualifiedName << curUnit->ns->name << "::" << classDecl.name;
    if (classDecl.fullQualifiedName != curUnit->name) {
        error() << curUnit->name << ": class name '" << classDecl.fullQualifiedName << "' does not match compilation unit\n";
    }
    Scope *tmpScope = curScope;
    ClassScope *classScope = tmpScope->registerClass(classDecl, classDecl.name);
    InstanceScope *instanceScope = &env().create<InstanceScope, Scope &, ClassDeclNode &>(*curScope, classDecl);
    curScope = classDecl.scope = classScope;
    classDecl.instanceScope = instanceScope;
    loader.registerClass(classDecl);

    {
        Iterator<String> &it = classDecl.fullQualifiedName.parts();
        while (it.hasNext()) {
            classDecl.globalPrefix << '_' << it.next();
        }
        it.destroy();
        (classDecl.localPrefix << '_').printuint(classDecl.fullQualifiedName.hash(), 16, 8);
    }
    
    Iterator<TypeRefNode> &it = classDecl.extends.iterator();
    while (it.hasNext()) {
        TypeRefNode & type = it.next();
        ClassRefNode *extend = type.isClass();
        if (!extend) {
            error() << curUnit->name << ": invalid extend type '" << type << "'\n";
        }
        extend->accept(*this);
        ClassScope *extendClassScope = extend->resolvedType;
        InstanceScope *extendInstanceScope = extendClassScope->getClassDeclNode()->instanceScope;
        
        {
            Iterator<ClassScope> &sit = extendClassScope->supers();
            while (sit.hasNext()) {
                ClassScope & super = sit.next();
                classScope->addSuper(super);
            }
            sit.destroy();
        }
        
        {
            Iterator<MethodScope> &mit = extendInstanceScope->methods();
            while (mit.hasNext()) {
                instanceScope->registerMethod(*mit.next().getMethodDeclNode());
            }
            mit.destroy();
        }
        {
            Iterator<MethodScope> &mit = extendClassScope->methods();
            while (mit.hasNext()) {
                classScope->registerMethod(*mit.next().getMethodDeclNode());
            }
            mit.destroy();
        }
    }
    it.destroy();
    
    classScope->addSuper(*classScope);
    {
        Iterator<MethodDeclNode> &mit = classDecl.methods.iterator();
        int index = 0;
        while (mit.hasNext()) {
            MethodDeclNode &methodDecl = mit.next();
            methodDecl.scope = (methodDecl.global ? (Scope*) classScope : (Scope*) instanceScope)->registerMethod(methodDecl);
            methodDecl.index = index++;
        }
        mit.destroy();
    }
    {
        Iterator<VariableDeclNode> &vit = classDecl.variables.iterator();
        while (vit.hasNext()) {
            VariableDeclNode &variableDecl = vit.next();
            variableDecl.scope = instanceScope->registerVariable(variableDecl);
        }
        vit.destroy();
    }
    {
        Iterator<VariableInitInstNode> &vit = classDecl.consts.iterator();
        while (vit.hasNext()) {
            VariableInitInstNode &variableInit = vit.next();
            variableInit.scope = classScope;
            VariableDeclNode *variableDecl = variableInit.variables.first();
            variableDecl->scope = classScope->registerVariable(*variableDecl);
        }
        vit.destroy();
    }
    
    int index = 0;
    {
        Iterator<MethodScope> &mit = instanceScope->methods();
        while (mit.hasNext()) {
            mit.next().index = index++;
        }
        mit.destroy();
    }
    {
        Iterator<MethodScope> &mit = classScope->methods();
        while (mit.hasNext()) {
            mit.next().index = index++;
        }
        mit.destroy();
    }
    
    if (curUnit->element.hasStringProperty("pool.bootstrap")) {
        String & bsName = curUnit->element.getStringProperty("pool.bootstrap");
        MethodScope * bsScope = classScope->getMethod(bsName);
        if (!bsScope) {
            warn() << curUnit->name << ": ignore missing global bootstrap method '" << bsName << "'\n";
            curUnit->element.unsetProperty("pool.bootstrap");
            curScope = tmpScope;
            return false;
        }
    }
    curScope = tmpScope;
    return true;
}
