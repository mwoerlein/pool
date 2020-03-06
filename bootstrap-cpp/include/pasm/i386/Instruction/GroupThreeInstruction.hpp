#ifndef PASM_I386_INSTRUCTION_GROUPTWOINSTRUCTION_HPP_LOCK
#define PASM_I386_INSTRUCTION_GROUPTWOINSTRUCTION_HPP_LOCK

#include "pasm/i386/ASMInstruction.hpp"

class GroupThreeInstruction: public ASMInstruction {
    protected:
    int regO;
    
    virtual size_t approximateSizeInBytes() override;
    virtual void checkOperands() override;
    virtual void validateOperands() override;
    virtual size_t compileOperands() override;
    virtual void writeOperandsToStream(OStream &stream) override;
    
    public:
    GroupThreeInstruction(Environment &env, MemoryInfo &mi, const char * mnemonic, int regO, ASMOperand *o1, BitWidth operandSize = bit_auto)
        :ASMInstruction(env, mi, mnemonic, operandSize, o1), Object(env, mi), regO(regO) {}
    virtual ~GroupThreeInstruction() {}
};

#endif //PASM_I386_INSTRUCTION_GROUPTWOINSTRUCTION_HPP_LOCK
