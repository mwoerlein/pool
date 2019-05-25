#ifndef PASM_I386_OPERAND_HPP_LOCK
#define PASM_I386_OPERAND_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/stream/OStream.hpp"
#include "pasm/i386/ASMTypes.hpp"

class ASMInstructionList;
class Numeric;
class ASMOperand: virtual public Object {
    public:
    virtual ~ASMOperand();
    
    virtual ASMOperand * validateAndReplace(ASMInstructionList & list, BitWidth mode);
    virtual OStream & operator >>(OStream & stream) = 0;
    
    // TODO #6: implement RTTI correctly
    virtual RTTI rtti() override;
    virtual Numeric * asNumeric();
    template <class As> As * as(OperandType type) {
        return (As *) ((this->type() == type) ? this->_memory_info->buf : 0);
    }
    virtual OperandType type() = 0;
};
//#include "pasm/i386/ASMInstructionList.hpp"
#include "pasm/i386/Operand/Numeric.hpp"

#endif //PASM_I386_OPERAND_HPP_LOCK
