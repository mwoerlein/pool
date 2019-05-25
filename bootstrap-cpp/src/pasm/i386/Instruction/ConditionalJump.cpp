#include "pasm/i386/Instruction/ConditionalJump.hpp"

#include "pasm/i386/Operand/Identifier.hpp"

// protected
size_t ConditionalJump::approximateSizeInBytes() {
    if (condition == cond_reg_cx || condition == cond_reg_ecx) {
        return 7; // opcode + 2*prefix + offset
    }
    return 6; // opcode + prefix + offset
}

void ConditionalJump::checkOperands() {
    if (!o1) {
        list->err<<"Missing operand!\n";
    }
    if (o2) {
        list->err<<"Unexpected operand: " << *o2 << '\n';
    }
    if (o3) {
        list->err<<"Unexpected operand: " << *o3 << '\n';
    }
    if (operandSize != bit_auto) {
        list->err<<"Invalid operand size!\n";
    }
}

void ConditionalJump::validateOperands() {
    Numeric *num1 = o1->asNumeric();
    
    if (condition == cond_reg_cx || condition == cond_reg_ecx) {
        Identifier *id1 = o1->as<Identifier>(identifier);
        if (!id1) {
            list->err<<"Only label based addressing allowed in: " << *this << '\n';
            return;
        }
        size_t endOffset = requiresAddressSizeOverride((condition == cond_reg_cx) ? bit_16 : bit_32) ? 3 : 2;
        if (approximateOffsetWidth(id1, endOffset) != bit_8) {
            list->err<<"Only byte offset allowed in: " << *this << '\n';
            return;
        }
    }
    
    if (num1) {
        return;
    }
    list->err<<"Invalid operand: " << *o1 << '\n';
    return;
}

size_t ConditionalJump::compileOperands() {
    Numeric *num1 = o1->asNumeric();
    if (num1) {
        size_t endOffset = 2;
        if (
            (condition == cond_reg_cx || condition == cond_reg_ecx)
            && requiresAddressSizeOverride((condition == cond_reg_cx) ? bit_16 : bit_32)
        ) {
            endOffset = 3;
        }
        size_t size = 1;
        // Offset of explicit address cannot be determined before final positioning in ASMInstructionList::finalize
        BitWidth offsetWidth = num1->isConstant(*list) ? ctx->addr : approximateOffsetWidth(num1, endOffset);
        if (offsetWidth == bit_8) {
            if (condition == cond_reg_cx || condition == cond_reg_ecx) {
                if (requiresAddressSizeOverride((condition == cond_reg_cx) ? bit_16 : bit_32)) {
                    pre4 = 0x67; size++;
                }
                op1 = 0xE3;
            } else {
                op1 = 0x70 + getConditionEncoding(condition);
            } 
        } else {
            // relative offsets must result in correct addr width, otherwise the upper two bytes of the EIP register are cleared
            offsetWidth = ctx->addr;
            op1 = 0x0F; size++;
            op2 = 0x80 + getConditionEncoding(condition);
        }
        immSize = (int) offsetWidth;
        return size + immSize;
    }
    return -1;
}

void ConditionalJump::writeOperandsToStream(OStream & stream) {
    if (immSize) {
        writeOffsetToStream(stream, o1);
    }
}

// private
const char* ConditionalJump::getMnemonic(InstructionCondition cond) {
    switch (cond) {
        case cond_above: return "ja";
        case cond_above_or_equal: return "jae";
        case cond_below: return "jb";
        case cond_below_or_equal: return "jbe";
        case cond_carry: return "jc";
        case cond_equal: return "je";
        case cond_greater: return "jg";
        case cond_greater_or_equal: return "jge";
        case cond_lesser: return "jl";
        case cond_lesser_or_equal: return "jle";
        case cond_overflow: return "jo";
        case cond_parity: return "jp";
        case cond_sign: return "js";
        case cond_zero: return "jz";
        
        case cond_not_above: return "jna";
        case cond_not_above_or_equal: return "jnae";
        case cond_not_below: return "jnb";
        case cond_not_below_or_equal: return "jnbe";
        case cond_not_carry: return "jnc";
        case cond_not_equal: return "jne";
        case cond_not_greater: return "jng";
        case cond_not_greater_or_equal: return "jnge";
        case cond_not_lesser: return "jnl";
        case cond_not_lesser_or_equal: return "jnle";
        case cond_not_overflow: return "jno";
        case cond_not_parity: return "jnp";
        case cond_not_sign: return "jns";
        case cond_not_zero: return "jnz";
        
        case cond_parity_even: return "jpe";
        case cond_parity_odd: return "jpo";
        
        case cond_reg_cx: return "jcxz"; 
        case cond_reg_ecx: return "jecxz"; 
    }
    return "j??";
}
