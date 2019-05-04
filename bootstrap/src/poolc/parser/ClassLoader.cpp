#include "poolc/parser/ClassLoader.hpp"

#include "poolc/ast/nodes/TranslationUnitNode.hpp"
#include "poolc/ast/visitors/ClassResolver.hpp"
#include "poolc/ast/visitors/PrettyPrinter.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/StructScope.hpp"

// public
ClassLoader::ClassLoader(Environment & env, MemoryInfo & mi, TypeManager & types)
        :ClassPathStorage(env, mi), LoggerAware(env, mi), HashMap(env, mi), Object(env, mi),
         parser(env.create<PoolParser>()),
         resolve(env.create<ClassResolver, ClassLoader &, TypeManager &>(*this, types)),
         classes(env.create<HashMap<String, ClassDeclNode>>()),
         structs(env.create<HashMap<String, StructDeclNode>>()),
         pretty(0) {
}

ClassLoader::~ClassLoader() {
    parser.destroy();
    resolve.destroy();
    classes.destroy();
    structs.destroy();
}
    
void ClassLoader::setLogger(Logger &logger) {
    LoggerAware::setLogger(logger);
    parser.setLogger(logger);
    resolve.setLogger(logger);
}

void ClassLoader::setPrettyPrint(PrettyPrinter &pretty) {
    this->pretty = &pretty;
}

void ClassLoader::registerClass(ClassDeclNode & classDecl) {
    classes.set(classDecl.fullQualifiedName, classDecl);
}

ClassDeclNode * ClassLoader::getClass(String & fullQualifiedName) {
    if (!classes.has(fullQualifiedName) && !structs.has(fullQualifiedName) && !this->initialize(fullQualifiedName)) {
        return 0;
    }
    if (classes.has(fullQualifiedName)) {
        return &classes.get(fullQualifiedName);
    }
    if (structs.has(fullQualifiedName)) {
        error() << "struct '" << fullQualifiedName << "' is not a class!\n";
        return 0;
    }
    error() << "class '" << fullQualifiedName << "' not declared!\n";
    return 0;
}

void ClassLoader::registerStruct(StructDeclNode & structDecl) {
    structs.set(structDecl.fullQualifiedName, structDecl);
}

NamedType * ClassLoader::getNamedType(String & fullQualifiedName) {
    if (!classes.has(fullQualifiedName) && !structs.has(fullQualifiedName) && !this->initialize(fullQualifiedName)) {
        return 0;
    }
    if (classes.has(fullQualifiedName)) {
        ClassDeclNode &decl = classes.get(fullQualifiedName);
        if (!decl.scope) {
            warn() << "unresolved class '" << fullQualifiedName << "' loaded!\n";
            return 0;
        }
        return decl.scope->isClass();
    }
    if (structs.has(fullQualifiedName)) {
        StructDeclNode &decl = structs.get(fullQualifiedName);
        if (!decl.scope) {
            warn() << "unresolved struct '" << fullQualifiedName << "' loaded!\n";
            return 0;
        }
        return decl.scope->isStruct();
    }
    warn() << "unknown type '" << fullQualifiedName << "' requested\n";
    return 0;
}


bool ClassLoader::initialize(String & fullQualifiedName) {
    StorageElement *e = ClassPathStorage::getElement(fullQualifiedName);
    if (!e) {
        error() << "declaration of '" << fullQualifiedName << "' not found!\n";
        return false;
    }
    
    debug() << "load and parse " << fullQualifiedName << "\n";
    
    IStream &in = e->getContent();
    TranslationUnitNode * unit = parser.parse(*e, fullQualifiedName);
    in.destroy();
    if (!unit) {
        return false;
    }
    if (pretty) { unit->accept(*pretty); }
    unit->accept(resolve);
    return true;
}
