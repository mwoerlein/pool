#ifndef PASM_I386_IDENTIFIER_HPP_LOCK
#define PASM_I386_IDENTIFIER_HPP_LOCK

#include "sys/String.hpp"
#include "pasm/i386/Operand/Numeric.hpp"

class Identifier: public Numeric {
    private:
    String &_id;
    
    public:
    Identifier(Environment &env, MemoryInfo &mi, String &id);
    virtual ~Identifier();
    
    virtual OStream & operator >>(OStream & stream);
    virtual String &id();
    
    virtual bool isConstant(ASMInstructionList & list) override;
    virtual int getValue(ASMInstructionList & list) override;
    virtual Numeric & clone() override;
    virtual Numeric * validateAndReplace(ASMInstructionList & list, BitWidth mode) override;
    
    // TODO #6: implement RTTI correctly
    virtual OperandType type() override;
    virtual int hash() override;
    virtual bool equals(Object &o) override;
};

#endif //PASM_I386_IDENTIFIER_HPP_LOCK
