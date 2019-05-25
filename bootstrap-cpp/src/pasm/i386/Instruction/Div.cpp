#include "pasm/i386/Instruction/Div.hpp"

// protected
size_t Div::approximateSizeInBytes() {
    Indirect *i1 = o1->as<Indirect>(indirect);
    
    size_t size = 2; //opcode, modrm
    if (requiresOperandSizeOverride()) {
        size++;
    }
    
    if (i1) {
        if (requiresAddressSizeOverride(i1)) {
            size++;
        }
        size += i1->getSibSize();
        size += (int) i1->getDispSize();
    } 
    return 1;
}

void Div::checkOperands() {
    if (!o1) {
        list->err<<"Missing operand!\n";
    }
    if (o2) {
        list->err<<"Unexpected operand: " << *o2 << '\n';
    }
    if (o3) {
        list->err<<"Unexpected operand: " << *o3 << '\n';
    }
    if (list->hasErrors()) return;
    
    if (operandSize == bit_auto) {
        if (Register *r1 = o1->as<Register>(reg)) {
            operandSize = r1->getOperandSize();
        } else {
            list->err<<"missing operand size and no general purpose register to determine from in \""<<*this<<"\"\n";
        }
    }
}

void Div::validateOperands() {
    Register *r1 = o1->as<Register>(reg);
    Indirect *i1 = o1->as<Indirect>(indirect);
    
    if (r1) {
        if (r1->kind() != reg_general) {
            list->err<<"Invalid register: " << *o1 << '\n';
        }
        r1->validate(*list, operandSize);
        return;
    }
    if (i1) {
        return;
    }
    
    list->err<<"Invalid operand: " << *o1 << '\n';
    return;
}

size_t Div::compileOperands() {
    size_t size = 0;
    Register *r1 = o1->as<Register>(reg);
    Indirect *i1 = o1->as<Indirect>(indirect);
    
    if (requiresOperandSizeOverride()) {
        pre3 = 0x66; size++;
    }
    
    if (r1) {
        op1 = (operandSize == bit_8) ? 0xF6 : 0xF7;
        modrmSize = 1;
        return size + 1 + modrmSize;
    }
    if (i1) {
        if (requiresAddressSizeOverride(i1)) {
            pre4 = 0x67; size++;
        }    
        op1 = (operandSize == bit_8) ? 0xF6 : 0xF7;
        useIndirectSizes(i1);
        return size + 1 + modrmSize + sibSize + dispSize;
    }
    return -1;
}

void Div::writeOperandsToStream(OStream & stream) {
    Register *r1 = o1->as<Register>(reg);
    Indirect *i1 = o1->as<Indirect>(indirect);
    if (r1) {
        writeModRMToStream(stream, 6, r1);
    }
    if (i1) {
        writeIndirectToStream(stream, i1, 6);
    }
}
