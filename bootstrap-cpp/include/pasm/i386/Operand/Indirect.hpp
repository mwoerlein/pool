#ifndef PASM_I386_INDIRECT_HPP_LOCK
#define PASM_I386_INDIRECT_HPP_LOCK

#include "pasm/i386/ASMOperand.hpp"

#include "pasm/i386/Operand/Numeric.hpp"
#include "pasm/i386/Operand/Register.hpp"

class Indirect: public ASMOperand {
    private:
    Numeric *_displacement;
    Register *_base;
    Register *_index;
    int _scale;
    BitWidth addrSize = bit_auto;
    
    public:
    Indirect(Environment &env, MemoryInfo &mi, Register *base = 0, Numeric *displacement = 0, Register * index = 0, int scale = 1);
    virtual ~Indirect();
        
    virtual ASMOperand * validateAndReplace(ASMInstructionList & list, BitWidth mode) override;
    virtual BitWidth getAddrSize();
    virtual int getModRMSize();
    virtual int getSibSize();
    virtual BitWidth getDispSize();
    virtual char getModRM(int reg);
    virtual char getSib();
    virtual int getDispValue(ASMInstructionList & list);
    virtual bool isOffset();
    virtual OStream & operator >>(OStream & stream);
    
    virtual OperandType type() override;
};

#endif //PASM_I386_INDIRECT_HPP_LOCK
