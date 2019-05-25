#ifndef PASM_I386_INSTRUCTION_GROUPONEINSTRUCTION_HPP_LOCK
#define PASM_I386_INSTRUCTION_GROUPONEINSTRUCTION_HPP_LOCK

#include "pasm/i386/ASMInstruction.hpp"

class GroupOneInstruction: public ASMInstruction {
    protected:
    int regO;
    
    virtual size_t approximateSizeInBytes() override;
    virtual void checkOperands() override;
    virtual void validateOperands() override;
    virtual size_t compileOperands() override;
    virtual void writeOperandsToStream(OStream &stream) override;
    
    public:
    GroupOneInstruction(Environment &env, MemoryInfo &mi, const char * mnemonic, int regO, ASMOperand *o1, ASMOperand *o2, BitWidth operandSize = bit_auto)
        :ASMInstruction(env, mi, mnemonic, operandSize, o1, o2), Object(env, mi), regO(regO) {}
    virtual ~GroupOneInstruction() {}
};

#endif //PASM_I386_INSTRUCTION_GROUPONEINSTRUCTION_HPP_LOCK
