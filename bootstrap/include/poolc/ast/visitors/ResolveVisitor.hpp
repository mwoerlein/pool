#ifndef POOLC_AST_VISITORS_RESOLVEVISITOR_HPP_LOCK
#define POOLC_AST_VISITORS_RESOLVEVISITOR_HPP_LOCK

#include "poolc/ast/Visitor.hpp"

#include "poolc/ast/SimpleFactory.hpp"

class ResolveVisitor: public Visitor {
    private:
    SimpleFactory & factory;
    
    public:
    ResolveVisitor(Environment &env, MemoryInfo &mi, SimpleFactory & factory);
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
