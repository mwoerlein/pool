#ifndef POOLC_AST_VISITORS_RESOLVEVISITOR_HPP_LOCK
#define POOLC_AST_VISITORS_RESOLVEVISITOR_HPP_LOCK

#include "poolc/ast/Visitor.hpp"

#include "poolc/parser/ClassLoader.hpp"

class ResolveVisitor: public Visitor {
    private:
    ClassLoader & loader;
    TranslationUnitNode * curUnit;
    
    public:
    ResolveVisitor(Environment &env, MemoryInfo &mi, ClassLoader & loader);
    virtual ~ResolveVisitor();
    
    virtual bool visit(TranslationUnitNode & translationUnit) override;

    virtual bool visit(NamespaceDeclNode & namespaceDef) override;
    virtual bool visit(UseStatementNode & useStmt) override;
    virtual bool visit(ClassDeclNode & classDef) override;
    
    virtual bool visit(ClassRefNode & classRef) override;
    virtual bool visit(CStringConstAssignNode & constDef) override;
    virtual bool visit(IntConstAssignNode & constDef) override;
    virtual bool visit(VariableDeclNode & variableDef) override;
    virtual bool visit(MethodRefNode & methodRef) override;
    virtual bool visit(MethodDeclNode & methodDef) override;
    
    virtual bool visit(InlinePasmInstructionNode & pasmInstruction) override;
};

#endif //POOLC_AST_VISITORS_RESOLVEVISITOR_HPP_LOCK
