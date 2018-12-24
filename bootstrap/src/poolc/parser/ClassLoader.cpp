#include "poolc/parser/ClassLoader.hpp"

#include "poolc/ast/nodes/TranslationUnitNode.hpp"
#include "poolc/ast/visitors/ResolveVisitor.hpp"

// public
ClassLoader::ClassLoader(Environment & env, MemoryInfo & mi)
        :ClassPathStorage(env, mi), LoggerAware(env, mi), HashMap(env, mi), Object(env, mi),
         parser(env.create<PoolParser>()),
         resolve(env.create<ResolveVisitor, ClassLoader &>(*this)) {
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
        
        unit->accept(resolve);
    }
    
    return &HashMap::get(fullQualifiedName);
}
