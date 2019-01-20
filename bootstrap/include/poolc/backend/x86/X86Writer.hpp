#ifndef POOLC_BACKEND_X86_X86WRITER_HPP_LOCK
#define POOLC_BACKEND_X86_X86WRITER_HPP_LOCK

#include "poolc/ast/visitors/Writer.hpp"

#include "poolc/pir/PIRBasicBlock.hpp"
#include "poolc/pir/PIRLocation.hpp"
#include "poolc/pir/PIRMethod.hpp"
#include "poolc/pir/PIRStatement.hpp"
#include "poolc/pir/statement/PIRArithOp.hpp"
#include "poolc/pir/statement/PIRAsm.hpp"
#include "poolc/pir/statement/PIRAssign.hpp"
#include "poolc/pir/statement/PIRCall.hpp"
#include "poolc/pir/statement/PIRGet.hpp"
#include "poolc/pir/statement/PIRMove.hpp"
#include "poolc/pir/statement/PIRSet.hpp"

class X86Writer: public Writer {
    private:
    ClassDeclNode * curClass;
    ClassDeclNode * curSuper;
    PIRMethod * curMethod;

    virtual OStream & code();
    virtual void pushAllReverse(Iterator<PIRLocation> &it);
    
    public:
    X86Writer(Environment &env, MemoryInfo &mi, PoolStorage &ps);
    virtual ~X86Writer();
    
    virtual bool visit(TranslationUnitNode & translationUnit) override;

    virtual bool visit(ClassDeclNode & classDef) override;
    virtual bool visit(MethodDeclNode & methodDef) override;
    virtual bool visit(VariableDeclNode & variableDef) override;
    virtual bool visit(VariableInitInstNode & variableInit) override;
    
    virtual void write(PIRBasicBlock &block);
    virtual void write(PIRCond &cond, PIRBasicBlock &trueBlock);
    virtual void write(PIRStatement &stmt);
    virtual void write(PIRArithOp &arithOpStmt);
    virtual void write(PIRAsm &asmStmt);
    virtual void write(PIRAssign &assignStmt);
    virtual void write(PIRCall &callStmt);
    virtual void write(PIRGet &getStmt);
    virtual void write(PIRMove &moveStmt);
    virtual void write(PIRSet &setStmt);
    
    virtual void write(PIRLocation &location);
};

#endif //POOLC_BACKEND_X86_X86WRITER_HPP_LOCK
