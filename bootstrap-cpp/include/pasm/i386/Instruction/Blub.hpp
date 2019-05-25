#ifndef PASM_I386_INSTRUCTION_BLUB_HPP_LOCK
#define PASM_I386_INSTRUCTION_BLUB_HPP_LOCK

#include "pasm/i386/ASMInstruction.hpp"

class Blub: public ASMInstruction {
    protected:
    virtual size_t approximateSizeInBytes() override;
    virtual void checkOperands() override;
    virtual void validateOperands() override;
    virtual size_t compileOperands() override;
    virtual void writeOperandsToStream(OStream &stream) override;
    
    public:
    Blub(Environment &env, MemoryInfo &mi, ASMOperand *o1, BitWidth operandSize = bit_auto)
        :ASMInstruction(env, mi, "BLUB", operandSize, o1), Object(env, mi) {}
    virtual ~Blub() {}
};

#endif //PASM_I386_INSTRUCTION_BLUB_HPP_LOCK
