#ifndef PASM_I386_INSTRUCTION_INT_HPP_LOCK
#define PASM_I386_INSTRUCTION_INT_HPP_LOCK

#include "pasm/i386/ASMInstruction.hpp"

class Int: public ASMInstruction {
    protected:
    virtual size_t approximateSizeInBytes() override;
    virtual void checkOperands() override;
    virtual void validateOperands() override;
    virtual size_t compileOperands() override;
    virtual void writeOperandsToStream(OStream &stream) override;
    
    public:
    Int(Environment &env, MemoryInfo &mi, ASMOperand *o1)
        :ASMInstruction(env, mi, "int", bit_auto, o1), Object(env, mi) {}
    virtual ~Int() {}
};

#endif //PASM_I386_INSTRUCTION_INT_HPP_LOCK
