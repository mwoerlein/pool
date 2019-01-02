#ifndef POOLC_BACKEND_X86_X86WRITER_HPP_LOCK
#define POOLC_BACKEND_X86_X86WRITER_HPP_LOCK

#include "poolc/ast/visitors/Writer.hpp"

class X86Writer: public Writer {
    private:
    ClassDeclNode * curClass;
    ClassDeclNode * curSuper;
    
    public:
    X86Writer(Environment &env, MemoryInfo &mi, PoolStorage &ps);
    virtual ~X86Writer();
    
    virtual bool visit(TranslationUnitNode & translationUnit) override;

    virtual bool visit(ClassDeclNode & classDef) override;
    virtual bool visit(MethodDeclNode & methodDef) override;
    virtual bool visit(VariableDeclNode & variableDef) override;
    
    virtual bool visit(BlockInstNode & block) override;
    virtual bool visit(InlinePasmInstNode & pasmInstruction) override;
    virtual bool visit(VariableInitInstNode & variableInit) override;
};

#endif //POOLC_BACKEND_X86_X86WRITER_HPP_LOCK
