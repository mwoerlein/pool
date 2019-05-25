#ifndef PASM_I386_INSTRUCTION_HPP_LOCK
#define PASM_I386_INSTRUCTION_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/stream/OStream.hpp"
#include "pasm/i386/ASMTypes.hpp"
#include "pasm/i386/ASMContext.hpp"
#include "pasm/i386/ASMInstructionList.hpp"
#include "pasm/i386/Operand/Numeric.hpp"
#include "pasm/i386/Operand/Indirect.hpp"
#include "pasm/i386/Operand/Register.hpp"

#define useIndirectSizes(i) { \
    modrmSize = (i)->getModRMSize();\
    sibSize = (i)->getSibSize();\
    dispSize = (int) (i)->getDispSize();\
}

class ASMInstruction: virtual public Object {
    protected:
    friend class ASMInstructionList;
    const char *mnemonic;
    ASMOperand *o1, *o2, *o3;
    BitWidth operandSize;
    
    char pre1, pre2, pre3, pre4;
    char op1, op2, op3;
    int modrmSize, sibSize, dispSize, immSize;
    size_t size;
    ASMContext *ctx;
    ASMInstructionList * list;
    
    virtual size_t approximateSizeInBytes();
    virtual void checkOperands();
    virtual void sanitizeOperands();
    virtual void validateOperands();
    virtual size_t compileOperands() = 0;
    virtual void writeOperandsToStream(OStream &stream) = 0;
    virtual void writeValueToStream(OStream &stream, int val, int size);
    virtual void writeOffsetToStream(OStream &stream, ASMOperand *o);
    virtual void writeImmediateToStream(OStream &stream, ASMOperand *o);
    virtual void writeModRMToStream(OStream &stream, int regO, int regM);
    virtual void writeIndirectToStream(OStream &stream, Indirect *i, int reg);
    virtual BitWidth getBitWidth(int value);
    virtual BitWidth getUnsignedBitWidth(unsigned int value);
    virtual BitWidth approximateOffsetWidth(Numeric *num, size_t posIncr = 0);
    virtual int getConditionEncoding(InstructionCondition cond);

    inline void writeModRMToStream(OStream &stream, Register *regO, Register *regM) {
        writeModRMToStream(stream, regO->getOpCodeRegister(), regM->getOpCodeRegister());
    }
    inline void writeModRMToStream(OStream &stream, int regO, Register *regM) {
        writeModRMToStream(stream, regO, regM->getOpCodeRegister());
    }
    inline void writeIndirectToStream(OStream &stream, Indirect *i, Register *reg) {
        writeIndirectToStream(stream, i, reg->getOpCodeRegister());
    }
    
    inline bool requiresOperandSizeOverride() { return ctx->requiresOperandSizeOverride(operandSize); }
    inline bool requiresOperandSizeOverride(BitWidth operandSize) { return ctx->requiresOperandSizeOverride(operandSize); }
    inline bool requiresAddressSizeOverride(Indirect *i) { return ctx->requiresAddressSizeOverride(i->getAddrSize()); }
    inline bool requiresAddressSizeOverride(BitWidth addressSize) { return ctx->requiresAddressSizeOverride(addressSize); }
    
    public:
    ASMInstruction(Environment &env, MemoryInfo &mi, const char * mnemonic, BitWidth operandSize = bit_auto, ASMOperand *o1 = 0, ASMOperand *o2 = 0, ASMOperand *o3 = 0);
    virtual ~ASMInstruction();
    
    virtual size_t prepare();
    virtual size_t compile();
    
    virtual void writeToStream(OStream &stream);
    virtual OStream & operator >>(OStream & stream) override;
};

// http://sparksandflames.com/files/x86InstructionChart.html

#endif //PASM_I386_INSTRUCTION_HPP_LOCK
