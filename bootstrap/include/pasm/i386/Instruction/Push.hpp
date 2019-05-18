#ifndef PASM_I386_INSTRUCTION_BLUB_HPP_LOCK
#define PASM_I386_INSTRUCTION_BLUB_HPP_LOCK

#include "pasm/i386/ASMInstruction.hpp"

class Push: public ASMInstruction {
    protected:
    virtual size_t approximateSizeInBytes() override;
    virtual void checkOperands() override;
    virtual void validateOperands() override;
    virtual size_t compileOperands() override;
    virtual void writeOperandsToStream(OStream &stream) override;
    
    public:
    Push(Environment &env, MemoryInfo &mi, ASMOperand *o1, BitWidth operandSize = bit_auto)
        :ASMInstruction(env, mi, "push", operandSize, o1), Object(env, mi) {}
    virtual ~Push() {}
};

#endif //PASM_I386_INSTRUCTION_PUSH_HPP_LOCK
