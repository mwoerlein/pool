#include "pasm/i386/Instruction/GroupOneInstruction.hpp"

// protected
size_t GroupOneInstruction::approximateSizeInBytes() {
    Numeric *num1 = o1->asNumeric();
    Indirect *i1 = o1->as<Indirect>(indirect);
    Indirect *i2 = o2->as<Indirect>(indirect);
    
    size_t size = 2; //opcode, modrm
    if (requiresOperandSizeOverride()) { size++; }
    
    if (num1) {
        size += (operandSize == bit_auto) ? (int) bit_32 : (int) operandSize;
    }
    if (i1) {
        if (requiresAddressSizeOverride(i1)) { size++; }
        size += i1->getSibSize();
        size += (int) i1->getDispSize();
    } 
    if (i2) {
        if (requiresAddressSizeOverride(i2)) { size++; }
        size += i2->getSibSize();
        size += (int) i2->getDispSize();
    }
    return size;
}

void GroupOneInstruction::checkOperands() {
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
    
    if (operandSize == bit_auto) {
        if (r1) {
            BitWidth newSize = r1->getOperandSize();
            if (r2) {
                if (newSize != r2->getOperandSize()) {
                    list->err<<"ambigious operand sizes in \""<<*this<<"\"\n";
                }
            }
            operandSize = newSize;
        } else if (r2) {
            operandSize = r2->getOperandSize();
        } else {
            list->err<<"missing operand size and no general purpose register to determine from in \""<<*this<<"\"\n";
        }
    }
}

void GroupOneInstruction::validateOperands() {
    Numeric *num1 = o1->asNumeric();
    Register *r1 = o1->as<Register>(reg);
    Register *r2 = o2->as<Register>(reg);
    Indirect *i1 = o1->as<Indirect>(indirect);
    Indirect *i2 = o2->as<Indirect>(indirect);
    
    if (r1) {
        if (r1->kind() != reg_general) {
            list->err<<"Invalid register: " << *o1 << '\n';
        }
        r1->validate(*list, operandSize);
    }
    if (r2) {
        if (r2->kind() != reg_general) {
            list->err<<"Invalid register: " << *o2 << '\n';
        }
        r2->validate(*list, operandSize);
    }
    if (list->hasErrors()) return;
    
    if ((num1 || r1) && (r2 || i2)) {
        return;
    }
    if (i1 && r2) {
        return;
    }
    list->err<<"unsupported operands in \""<<*this<<"\"\n";
}

size_t GroupOneInstruction::compileOperands() {
    size_t size = 0;
    Numeric *num1 = o1->asNumeric();
    Register *r1 = o1->as<Register>(reg);
    Register *r2 = o2->as<Register>(reg);
    Indirect *i1 = o1->as<Indirect>(indirect);
    Indirect *i2 = o2->as<Indirect>(indirect);
    
    if (requiresOperandSizeOverride()) {
        pre3 = 0x66; size++;
    }
    
    if (num1 && (r2 || i2)) {
        immSize = (int) operandSize;
        if (r2 && (r2->getOpCodeRegister() == 0 /*al, ax, eax*/)) {
            op1 = (regO << 3) + ((operandSize == bit_8) ? 0x04 : 0x05);
            return size + 1 + immSize;
        }
        if (operandSize == bit_8) {
            op1 = 0x80;
        } else {
            op1 = 0x81;
            if (num1->isConstant(*list) && (getBitWidth(num1->getValue(*list)) == bit_8)) {
                op1 = 0x83;
                immSize = (int) bit_8;
            }
        }
        modrmSize = 1;
        if (i2) {
            if (requiresAddressSizeOverride(i2)) {
                pre4 = 0x67; size++;
            }    
            useIndirectSizes(i2);
        }
        return size + 1 + modrmSize + sibSize + dispSize + immSize;
    }
    if (r1 && (r2 || i2)) {
        op1 = (regO << 3) + ((operandSize == bit_8) ? 0x00 : 0x01);
        modrmSize = 1;
        if (i2) {
            if (requiresAddressSizeOverride(i2)) {
                pre4 = 0x67; size++;
            }    
            useIndirectSizes(i2);
        }
        return size + 1 + modrmSize + sibSize + dispSize;
    }
    if (i1 && r2) {
        op1 = (regO << 3) + ((operandSize == bit_8) ? 0x02 : 0x03);
        if (requiresAddressSizeOverride(i1)) {
            pre4 = 0x67; size++;
        }    
        useIndirectSizes(i1);
        return size + 1 + modrmSize + sibSize + dispSize;
    }
    return -1;
}

void GroupOneInstruction::writeOperandsToStream(OStream & stream) {
    Register *r1 = o1->as<Register>(reg);
    Register *r2 = o2->as<Register>(reg);
    Indirect *i1 = o1->as<Indirect>(indirect);
    Indirect *i2 = o2->as<Indirect>(indirect);
    
    if (immSize) {
        if (r2) {
            writeModRMToStream(stream, regO, r2);
        }
        if (i2) {
            writeIndirectToStream(stream, i2, regO);
        }
        writeImmediateToStream(stream, o1);
    }
    if (r1 && r2) {
        writeModRMToStream(stream, r1, r2);
    }
    if (r1 && i2) {
        writeIndirectToStream(stream, i2, r1);
    }
    if (i1 && r2) {
        writeIndirectToStream(stream, i1, r2);
    }
}
