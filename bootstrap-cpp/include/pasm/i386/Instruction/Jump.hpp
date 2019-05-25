#ifndef PASM_I386_INSTRUCTION_JUMP_HPP_LOCK
#define PASM_I386_INSTRUCTION_JUMP_HPP_LOCK

#include "pasm/i386/ASMInstruction.hpp"

class Jump: public ASMInstruction {
    protected:
    virtual size_t approximateSizeInBytes() override;
    virtual void checkOperands() override;
    virtual void validateOperands() override;
    virtual size_t compileOperands() override;
    virtual void writeOperandsToStream(OStream &stream) override;
    
    public:
    Jump(Environment &env, MemoryInfo &mi, ASMOperand *o1)
        :ASMInstruction(env, mi, "jmp", bit_auto, o1), Object(env, mi) {}
    virtual ~Jump() {}
};

#endif //PASM_I386_INSTRUCTION_JUMP_HPP_LOCK
