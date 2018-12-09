#ifndef POOLC_AST_VISITORS_X86PASMVISITOR_HPP_LOCK
#define POOLC_AST_VISITORS_X86PASMVISITOR_HPP_LOCK

#include "poolc/ast/Visitor.hpp"

#include "sys/stream/OStream.hpp"

class X86PasmVisitor: public Visitor {
    private:
    OStream & out;
    ClassDefNode * curClass;
    ClassDefNode * curSuper;
    
    public:
    X86PasmVisitor(Environment &env, MemoryInfo &mi, OStream &out);
    virtual ~X86PasmVisitor();
    
    virtual bool visit(ClassRefNode & classRef) override;
    virtual bool visit(ClassDefNode & classDef) override;
    virtual bool visit(MethodRefNode & methodRef) override;
    virtual bool visit(MethodDefNode & methodDef) override;
    virtual bool visit(VariableDefNode & variableDef) override;
    virtual bool visit(CStringConstDefNode & constDef) override;
    virtual bool visit(IntConstDefNode & constDef) override;
    
    virtual bool visit(InlinePasmInstructionNode & pasmInstruction) override;
};

#endif //POOLC_AST_VISITORS_X86PASMVISITOR_HPP_LOCK
