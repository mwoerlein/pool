#ifndef POOLC_AST_VISITORS_RESOLVEVISITOR_HPP_LOCK
#define POOLC_AST_VISITORS_RESOLVEVISITOR_HPP_LOCK

#include "poolc/ast/Visitor.hpp"

#include "poolc/ast/Scope.hpp"
#include "poolc/parser/ClassLoader.hpp"

class ResolveVisitor: public Visitor {
    private:
    Scope * curScope;
    ClassLoader & loader;
    
    public:
    ResolveVisitor(Environment &env, MemoryInfo &mi, ClassLoader & loader);
    virtual ~ResolveVisitor();
    
    virtual bool visit(TranslationUnitNode & translationUnit) override;

    virtual bool visit(ClassDeclNode & classDef) override;
    
    virtual bool visit(ClassRefNode & classRef) override;
    virtual bool visit(UseStatementNode & useStmt) override;
};

#endif //POOLC_AST_VISITORS_RESOLVEVISITOR_HPP_LOCK
