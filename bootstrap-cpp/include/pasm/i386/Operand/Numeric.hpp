#ifndef PASM_I386_OPERAND_NUMERIC_HPP_LOCK
#define PASM_I386_OPERAND_NUMERIC_HPP_LOCK

#include "pasm/i386/ASMOperand.hpp"

class Numeric: public ASMOperand {
    public:
    Numeric(Environment &env, MemoryInfo &mi):Object(env, mi) {}
    virtual ~Numeric() {}
    
    virtual Numeric * validateAndReplace(ASMInstructionList & list, BitWidth mode) override {
        return 0;
    }
    virtual bool isConstant(ASMInstructionList & list) = 0;
    virtual int getValue(ASMInstructionList & list) = 0;
    virtual Numeric & clone() = 0;
    virtual Numeric * asNumeric() {
        return this;
    }
};

#endif //PASM_I386_OPERAND_FORMULA_HPP_LOCK
