#include "poolc/parser/ClassLoader.hpp"

#include "poolc/ast/nodes/TranslationUnitNode.hpp"
#include "poolc/ast/visitors/ResolveVisitor.hpp"

// public
ClassLoader::ClassLoader(Environment & env, MemoryInfo & mi)
        :ClassPathStorage(env, mi), HashMap(env, mi), Object(env, mi),
         parser(env.create<PoolParser>()),
         resolve(env.create<ResolveVisitor, ClassLoader &>(*this)) {
}

ClassLoader::~ClassLoader() {
    parser.destroy();
    resolve.destroy();
}

void ClassLoader::registerClass(ClassDefNode & classDef) {
    HashMap::set(classDef.fullQualifiedName, classDef);
}

ClassDefNode * ClassLoader::getClass(String & fullQualifiedName) {
    if (!HashMap::has(fullQualifiedName)) {
        StorageElement *e = ClassPathStorage::getElement(fullQualifiedName);
        if (!e) {
            env().err() << "class '" << fullQualifiedName << "' not found!\n";
            return 0;
        }
        
//        env().out()<<"load and parse "<<fullQualifiedName<<"\n";
        
        IStream &in = e->getContent();
        TranslationUnitNode * unit = parser.parse(in, fullQualifiedName);
        in.destroy();
        if (!unit) {
            return 0;
        }
        
        unit->accept(resolve);
    }
    
    return &HashMap::get(fullQualifiedName);
}
