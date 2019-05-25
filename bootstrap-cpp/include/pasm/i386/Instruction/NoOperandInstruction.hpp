#ifndef PASM_I386_INSTRUCTION_NOOPERANDINSTRUCTION_HPP_LOCK
#define PASM_I386_INSTRUCTION_NOOPERANDINSTRUCTION_HPP_LOCK

#include "pasm/i386/ASMInstruction.hpp"

class NoOperandInstruction: public ASMInstruction {
    protected:
    virtual size_t approximateSizeInBytes() override;
    virtual void checkOperands() override;
    virtual size_t compileOperands() override;
    virtual void writeOperandsToStream(OStream &stream) override;
    
    public:
    NoOperandInstruction(Environment &env, MemoryInfo &mi, const char * mnemonic, char opCode1 = 90, char opCode2 = 0, char opCode3 = 0, BitWidth operandSize = bit_auto);
    virtual ~NoOperandInstruction();
};

#endif //PASM_I386_INSTRUCTION_NOOPERANDINSTRUCTION_HPP_LOCK
