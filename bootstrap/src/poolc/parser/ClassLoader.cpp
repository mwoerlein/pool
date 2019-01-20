#include "poolc/parser/ClassLoader.hpp"

#include "poolc/ast/nodes/TranslationUnitNode.hpp"
#include "poolc/ast/visitors/ClassResolver.hpp"
#include "poolc/ast/visitors/PrettyPrinter.hpp"

// public
ClassLoader::ClassLoader(Environment & env, MemoryInfo & mi, TypeManager & types)
        :ClassPathStorage(env, mi), LoggerAware(env, mi), HashMap(env, mi), Object(env, mi),
         parser(env.create<PoolParser>()),
         resolve(env.create<ClassResolver, ClassLoader &, TypeManager &>(*this, types)),
         pretty(0) {
}

ClassLoader::~ClassLoader() {
    parser.destroy();
    resolve.destroy();
}
    
void ClassLoader::setLogger(Logger &logger) {
    LoggerAware::setLogger(logger);
    parser.setLogger(logger);
    resolve.setLogger(logger);
}

void ClassLoader::setPrettyPrint(PrettyPrinter &pretty) {
    this->pretty = &pretty;
}

void ClassLoader::registerClass(ClassDeclNode & classDef) {
    HashMap::set(classDef.fullQualifiedName, classDef);
}

ClassDeclNode * ClassLoader::getClass(String & fullQualifiedName) {
    if (!HashMap::has(fullQualifiedName)) {
        StorageElement *e = ClassPathStorage::getElement(fullQualifiedName);
        if (!e) {
            error() << "class '" << fullQualifiedName << "' not found!\n";
            return 0;
        }
        
        debug() << "load and parse " << fullQualifiedName << "\n";
        
        IStream &in = e->getContent();
        TranslationUnitNode * unit = parser.parse(*e, fullQualifiedName);
        in.destroy();
        if (!unit) {
            return 0;
        }
        if (pretty) { unit->accept(*pretty); }
        unit->accept(resolve);
        if (!HashMap::has(fullQualifiedName)) {
            error() << "class '" << fullQualifiedName << "' not declared!\n";
            return 0;
        }
    }
    
    return &HashMap::get(fullQualifiedName);
}
