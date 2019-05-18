#include "pasm/i386/ASMOperand.hpp"

// public
ASMOperand::~ASMOperand() {}
    
ASMOperand * ASMOperand::validateAndReplace(ASMInstructionList & list, BitWidth mode) { return 0; }
Numeric * ASMOperand::asNumeric() { return 0; }
RTTI ASMOperand::rtti() { return asm_operand; }
