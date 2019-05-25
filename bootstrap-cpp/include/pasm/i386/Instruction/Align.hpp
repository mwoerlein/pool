#ifndef PASM_I386_INSTRUCTION_ALIGN_HPP_LOCK
#define PASM_I386_INSTRUCTION_ALIGN_HPP_LOCK

#include "pasm/i386/ASMInstruction.hpp"

class Align: public ASMInstruction {
    protected:
    virtual size_t approximateSizeInBytes() override;
    virtual void checkOperands() override;
    virtual void validateOperands() override;
    virtual size_t compileOperands() override;
    virtual void writeOperandsToStream(OStream &stream) override;
    
    public:
    Align(Environment &env, MemoryInfo &mi, ASMOperand *o1)
        :ASMInstruction(env, mi, ".align", bit_auto, o1), Object(env, mi) {}
    virtual ~Align() {}
    
    virtual void writeToStream(OStream &stream) override;
    virtual OStream & operator >>(OStream & stream) override;
};

#endif //PASM_I386_INSTRUCTION_ALIGN_HPP_LOCK
