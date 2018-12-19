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

    virtual bool visit(NamespaceDefNode & namespaceDef) override;
    virtual bool visit(UseStatementNode & useStmt) override;
    virtual bool visit(ClassDefNode & classDef) override;
    
    virtual bool visit(ClassRefNode & classRef) override;
    virtual bool visit(CStringConstDefNode & constDef) override;
    virtual bool visit(IntConstDefNode & constDef) override;
    virtual bool visit(VariableDefNode & variableDef) override;
    virtual bool visit(MethodRefNode & methodRef) override;
    virtual bool visit(MethodDefNode & methodDef) override;
    
    virtual bool visit(InlinePasmInstructionNode & pasmInstruction) override;
};

#endif //POOLC_AST_VISITORS_RESOLVEVISITOR_HPP_LOCK
