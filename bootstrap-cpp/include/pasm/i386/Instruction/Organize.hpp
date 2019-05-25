#ifndef PASM_I386_INSTRUCTION_ORGANIZE_HPP_LOCK
#define PASM_I386_INSTRUCTION_ORGANIZE_HPP_LOCK

#include "pasm/i386/ASMInstruction.hpp"

class Organize: public ASMInstruction {
    protected:
    virtual size_t approximateSizeInBytes() override;
    virtual void checkOperands() override;
    virtual void validateOperands() override;
    virtual size_t compileOperands() override;
    virtual void writeOperandsToStream(OStream &stream) override;
    
    public:
    Organize(Environment &env, MemoryInfo &mi, ASMOperand *o1)
        :ASMInstruction(env, mi, ".org", bit_auto, o1), Object(env, mi) {}
    virtual ~Organize() {}
    
    virtual void writeToStream(OStream &stream) override;
    virtual OStream & operator >>(OStream & stream) override;
};

#endif //PASM_I386_INSTRUCTION_ORGANIZE_HPP_LOCK
