#ifndef PASM_I386_OPERAND_FORMULA_HPP_LOCK
#define PASM_I386_OPERAND_FORMULA_HPP_LOCK

#include "pasm/i386/Operand/Numeric.hpp"

enum FormulaOperation { op_add, op_sub, op_mul, op_div, op_mod, op_shr, op_shl };

class Formula: public Numeric {
    private:
    const char* getOperation(FormulaOperation op);
    
    FormulaOperation _op;
    Numeric *_o1, *_o2;
    
    protected:
    virtual int compute(int v1, int v2);
    
    public:
    Formula(Environment &env, MemoryInfo &mi, FormulaOperation op, Numeric &o1, Numeric &o2);
    virtual ~Formula();

    virtual OStream & operator >>(OStream & stream);
        
    virtual bool isConstant(ASMInstructionList & list) override;
    virtual int getValue(ASMInstructionList & list) override;
    virtual Numeric * validateAndReplace(ASMInstructionList & list, BitWidth mode) override;
    virtual Numeric & clone() override;
    
    // TODO #6: implement RTTI correctly
    virtual OperandType type() override;
    virtual int hash() override;
    virtual bool equals(Object &o) override;
};

#endif //PASM_I386_OPERAND_FORMULA_HPP_LOCK
