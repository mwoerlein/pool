#ifndef PASM_I386_REGISTER_HPP_LOCK
#define PASM_I386_REGISTER_HPP_LOCK

#include "pasm/i386/ASMOperand.hpp"

class Register: public ASMOperand {
    private:
    RegisterName _reg;
    
    public:
    Register(Environment &env, MemoryInfo &mi, RegisterName reg);
    virtual ~Register();
    
    virtual BitWidth getOperandSize();
    virtual RegisterKind kind();
    virtual int getOpCodeRegister();
    virtual void validate(ASMInstructionList & list, BitWidth operandSize);
    virtual RegisterName wideRegister(RegisterName orig, BitWidth size);
    virtual OStream & operator >>(OStream & stream);
    
    inline RegisterName name() { return _reg; }
    inline bool operator ==(RegisterName reg) { return _reg == reg; }
    inline bool operator !=(RegisterName reg) { return _reg != reg; }

    // TODO #6: implement RTTI correctly
    virtual OperandType type() override;
    virtual int hash() override;
    virtual bool equals(Object &o) override;
};

#endif //PASM_I386_REGISTER_HPP_LOCK
