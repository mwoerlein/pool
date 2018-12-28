#ifndef POOLC_AST_VISITORS_X86PASMVISITOR_HPP_LOCK
#define POOLC_AST_VISITORS_X86PASMVISITOR_HPP_LOCK

#include "poolc/ast/visitors/Writer.hpp"

class X86PasmVisitor: public Writer {
    private:
    ClassDeclNode * curClass;
    ClassDeclNode * curSuper;
    
    public:
    X86PasmVisitor(Environment &env, MemoryInfo &mi, PoolStorage &ps);
    virtual ~X86PasmVisitor();
    
    virtual bool visit(TranslationUnitNode & translationUnit) override;

    virtual bool visit(ClassDeclNode & classDef) override;
    virtual bool visit(MethodDeclNode & methodDef) override;
    virtual bool visit(VariableDeclNode & variableDef) override;
    
    virtual bool visit(BlockInstNode & block) override;
    virtual bool visit(InlinePasmInstNode & pasmInstruction) override;
    virtual bool visit(VariableInitInstNode & variableInit) override;
};

#endif //POOLC_AST_VISITORS_X86PASMVISITOR_HPP_LOCK
