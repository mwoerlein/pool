#ifndef PASM_I386_INSTRUCTION_OUT_HPP_LOCK
#define PASM_I386_INSTRUCTION_OUT_HPP_LOCK

#include "pasm/i386/ASMInstruction.hpp"

class Out: public ASMInstruction {
    protected:
    virtual size_t approximateSizeInBytes() override;
    virtual void checkOperands() override;
    virtual void validateOperands() override;
    virtual size_t compileOperands() override;
    virtual void writeOperandsToStream(OStream &stream) override;
    
    public:
    Out(Environment &env, MemoryInfo &mi, ASMOperand *o1, ASMOperand *o2, BitWidth operandSize = bit_auto)
        :ASMInstruction(env, mi, "out", operandSize, o1, o2), Object(env, mi) {}
    virtual ~Out() {}
};

#endif //PASM_I386_INSTRUCTION_OUT_HPP_LOCK
