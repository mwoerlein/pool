#include "pasm/i386/Instruction/Move.hpp"

// protected
size_t Move::approximateSizeInBytes() {
    Numeric *num1 = o1->asNumeric();
    Indirect *i1 = o1->as<Indirect>(indirect);
    Indirect *i2 = o2->as<Indirect>(indirect);
    
    size_t size = 2; //opcode, modrm
    if (requiresOperandSizeOverride()) {
        size++;
    }
    
    if (num1) {
        size += (operandSize == bit_auto) ? (int) bit_32 : (int) operandSize;
    }
    if (i1) {
        if (requiresAddressSizeOverride(i1)) {
            size++;
        }
        size += i1->getSibSize();
        size += (int) i1->getDispSize();
    } 
    if (i2) {
        if (requiresAddressSizeOverride(i2)) {
            size++;
        }
        size += i2->getSibSize();
        size += (int) i2->getDispSize();
    }
    return size;
}

void Move::checkOperands() {
    if (!o1) {
        list->err<<"Missing source!\n";
    }
    if (!o2) {
        list->err<<"Missing destination!\n";
    }
    if (o3) {
        list->err<<"Unexpected operand: " << *o3 << '\n';
    }
    if (list->hasErrors()) return;
    
    Register *r1 = o1->as<Register>(reg);
    Register *r2 = o2->as<Register>(reg);
    Register *gr1 = (r1 && (r1->kind() == reg_general)) ? r1 : 0;
    Register *gr2 = (r2 && (r2->kind() == reg_general)) ? r2 : 0;
    Register *sr1 = (r1 && (r1->kind() == reg_segment)) ? r1 : 0;
    Register *sr2 = (r2 && (r2->kind() == reg_segment)) ? r2 : 0;
    
    if (operandSize == bit_auto) {
        if (gr1) {
            BitWidth newSize = gr1->getOperandSize();
            if (gr2) {
                if (newSize != gr2->getOperandSize()) {
                    list->err<<"ambigious operand sizes in \""<<*this<<"\"\n";
                }
            }
            operandSize = newSize;
        } else if (gr2) {
            operandSize = gr2->getOperandSize();
        } else if (sr1) {
            operandSize = sr1->getOperandSize();
        } else if (sr2) {
            operandSize = sr2->getOperandSize();
        } else {
            list->err<<"missing operand size and no general purpose register to determine from in \""<<*this<<"\"\n";
        }
    }
}

void Move::validateOperands() {
    Numeric *num1 = o1->asNumeric();
    Register *r1 = o1->as<Register>(reg);
    Register *r2 = o2->as<Register>(reg);
    Indirect *i1 = o1->as<Indirect>(indirect);
    Indirect *i2 = o2->as<Indirect>(indirect);
    Register *gr1 = (r1 && (r1->kind() == reg_general)) ? r1 : 0;
    Register *gr2 = (r2 && (r2->kind() == reg_general)) ? r2 : 0;
    Register *sr1 = (r1 && (r1->kind() == reg_segment)) ? r1 : 0;
    Register *sr2 = (r2 && (r2->kind() == reg_segment)) ? r2 : 0;
    
    if (gr1) {
        gr1->validate(*list, operandSize);
    }
    if (gr2) {
        gr2->validate(*list, operandSize);
    }
    if (list->hasErrors()) return;
    
    if ((num1 || gr1) && (gr2 || i2)) {
        return;
    }
    if (i1 && gr2) {
        return;
    }
    if (gr1 && sr2) {
        return;
    }
    if (i1 && sr2) {
        if (operandSize != bit_16) {
            list->err<<"invalid operand size in \""<<*this<<"\"\n";
        }
        return;
    }
    if (sr1 && gr2) {
        return;
    }
    if (sr1 && i2) {
        if (operandSize != bit_16) {
            list->err<<"invalid operand size in \""<<*this<<"\"\n";
        }
        return;
    }
    list->err<<"unsupported operands in \""<<*this<<"\"\n";
}

size_t Move::compileOperands() {
    size_t size = 0;
    Numeric *num1 = o1->asNumeric();
    Register *r1 = o1->as<Register>(reg);
    Register *r2 = o2->as<Register>(reg);
    Indirect *i1 = o1->as<Indirect>(indirect);
    Indirect *i2 = o2->as<Indirect>(indirect);
    Register *gr1 = (r1 && (r1->kind() == reg_general)) ? r1 : 0;
    Register *gr2 = (r2 && (r2->kind() == reg_general)) ? r2 : 0;
    Register *sr1 = (r1 && (r1->kind() == reg_segment)) ? r1 : 0;
    Register *sr2 = (r2 && (r2->kind() == reg_segment)) ? r2 : 0;
    
    if (requiresOperandSizeOverride()) {
        pre3 = 0x66; size++;
    }
    
    if (num1 && gr2) {
        immSize = (int) operandSize;
        op1 = (operandSize == bit_8) ? 0xB0 : 0xB8;
        op1 += gr2->getOpCodeRegister();
        return size + 1 + immSize;
    }
    if (num1 && i2) {
        if (requiresAddressSizeOverride(i2)) {
            pre4 = 0x67; size++;
        }    
        immSize = (int) operandSize;
        op1 = (operandSize == bit_8) ? 0xC6 : 0xC7;
        useIndirectSizes(i2);
        return size + 1 + modrmSize + sibSize + dispSize + immSize;
    }
    if (gr1 && gr2) {
        op1 = (operandSize == bit_8) ? 0x88 : 0x89;
        modrmSize = 1;
        return size + 1 + modrmSize;
    }
    if (gr1 && i2) {
        if (requiresAddressSizeOverride(i2)) {
            pre4 = 0x67; size++;
        }    
        if (i2->isOffset() && (gr1->getOpCodeRegister() == 0 /*al, ax, eax*/)) {
            op1 = (operandSize == bit_8) ? 0xA2 : 0xA3;
            dispSize = (int) i2->getAddrSize();
            return size + 1 + dispSize;
        }
        op1 = (operandSize == bit_8) ? 0x88 : 0x89;
        useIndirectSizes(i2);
        return size + 1 + modrmSize + sibSize + dispSize;
    }
    if (i1 && gr2) {
        if (requiresAddressSizeOverride(i1)) {
            pre4 = 0x67; size++;
        }    
        if (i1->isOffset() && (gr2->getOpCodeRegister() == 0 /*al, ax, eax*/)) {
            op1 = (operandSize == bit_8) ? 0xA0 : 0xA1;
            dispSize = (int) i1->getAddrSize();
            return size + 1 + dispSize;
        }
        op1 = (operandSize == bit_8) ? 0x8A : 0x8B;
        useIndirectSizes(i1);
        return size + 1 + modrmSize + sibSize + dispSize;
    }
    if (gr1 && sr2) {
        op1 = 0x8E;
        modrmSize = 1;
        return size + 1 + modrmSize;
    }
    if (i1 && sr2) {
        if (pre3) {
            pre3 = 0; size--; // segment register implicit uses 16 bit
        } 
        if (requiresAddressSizeOverride(i1)) {
            pre4 = 0x67; size++;
        }    
        op1 = 0x8E;
        useIndirectSizes(i1);
        return size + 1 + modrmSize + sibSize + dispSize;
    }
    if (sr1 && gr2) {
        op1 = 0x8C;
        modrmSize = 1;
        return size + 1 + modrmSize;
    }
    if (sr1 && i2) {
        if (pre3) {
            pre3 = 0; size--; // segment register implicit uses 16 bit
        } 
        if (requiresAddressSizeOverride(i2)) {
            pre4 = 0x67; size++;
        }    
        op1 = 0x8C;
        useIndirectSizes(i2);
        return size + 1 + modrmSize + sibSize + dispSize;
    }
    return -1;
}

void Move::writeOperandsToStream(OStream & stream) {
    Register *r1 = o1->as<Register>(reg);
    Register *r2 = o2->as<Register>(reg);
    Indirect *i1 = o1->as<Indirect>(indirect);
    Indirect *i2 = o2->as<Indirect>(indirect);
    
    if (immSize) {
        if (r2) {
            writeModRMToStream(stream, 0, r2);
        }
        if (i2) {
            writeIndirectToStream(stream, i2, 0);
        }
        writeImmediateToStream(stream, o1);
    }
    if (r1) {
        if (r2) {
            if (r2->kind() == reg_segment) {
                writeModRMToStream(stream, r2, r1);
            } else {
                writeModRMToStream(stream, r1, r2);
            }
        }
        if (i2) {
            writeIndirectToStream(stream, i2, r1);
        }
    }
    if (i1 && r2) {
        writeIndirectToStream(stream, i1, r2);
    }
}
