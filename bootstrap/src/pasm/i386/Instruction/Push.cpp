#include "pasm/i386/Instruction/Push.hpp"

// protected
size_t Push::approximateSizeInBytes() {
    size_t size = 1; // op1
    Numeric *num1 = o1->asNumeric();
    Indirect *i1 = o1->as<Indirect>(indirect);
    Register *r1 = o1->as<Register>(reg);
    Register *sr1 = (r1 && (r1->kind() == reg_segment)) ? r1 : 0;
    
    if (requiresOperandSizeOverride()) { size++; }
    
    if (num1) {
        if (num1->isConstant(*list) && (getBitWidth(num1->getValue(*list)) == bit_8)) {
            size += (int) bit_8;
        } else {
            size += (int) operandSize;
        }
    }
    if (i1) {
        if (requiresAddressSizeOverride(i1)) { size++; }
        size++; // modRM
        size += i1->getSibSize();
        size += (int) i1->getDispSize();
    }
    if (sr1) {
        if ((*sr1 == reg_fs) || (*sr1 == reg_gs)) { size++; } // op2
    }
    return size;
}

void Push::checkOperands() {
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
        Register *r1 = o1->as<Register>(reg);
        if (r1 && (r1->kind() == reg_general)) {
            operandSize = r1->getOperandSize();
        } else {
            operandSize = ctx->addr;
        }
    }
    switch (operandSize) {
        case bit_8:
            list->err<<"invalid operand size in \""<<*this<<"\"\n";
    }
}

void Push::validateOperands() {
    Numeric *num1 = o1->asNumeric();
    Indirect *i1 = o1->as<Indirect>(indirect);
    Register *r1 = o1->as<Register>(reg);
    
    if (r1 && (r1->kind() == reg_general)) {
        r1->validate(*list, operandSize);
    }
    if (list->hasErrors()) return;
    
    if (num1 || r1 || i1) {
        return;
    }
    list->err<<"unsupported operands in \""<<*this<<"\"\n";
}

size_t Push::compileOperands() {
    size_t size = 1; // op1
    Numeric *num1 = o1->asNumeric();
    Indirect *i1 = o1->as<Indirect>(indirect);
    Register *r1 = o1->as<Register>(reg);
    Register *gr1 = (r1 && (r1->kind() == reg_general)) ? r1 : 0;
    Register *sr1 = (r1 && (r1->kind() == reg_segment)) ? r1 : 0;
    
    if (requiresOperandSizeOverride()) {
        pre3 = 0x66; size++;
    }
    
    if (num1) {
        op1 = 0x68;
        immSize = (int) operandSize;
        if (num1->isConstant(*list) && (getBitWidth(num1->getValue(*list)) == bit_8)) {
            op1 = 0x6A;
            immSize = (int) bit_8;
        }
        return size + immSize;
    }
    if (gr1) {
        op1 = 0x50 + gr1->getOpCodeRegister();
        return size;
    }
    if (i1) {
        op1 = 0xFF;
        if (requiresAddressSizeOverride(i1)) {
            pre4 = 0x67; size++;
        }    
        useIndirectSizes(i1);
        return size + modrmSize + sibSize + dispSize;
    }
    if (sr1) {
        if (*sr1 == reg_cs) {
            op1 = 0x0E;
        } else if (*sr1 == reg_ds) {
            op1 = 0x1E;
        } else if (*sr1 == reg_es) {
            op1 = 0x06;
        } else if (*sr1 == reg_fs) {
            op1 = 0x0F;
            op2 = 0xA0; size++;
        } else if (*sr1 == reg_gs) {
            op1 = 0x0F;
            op2 = 0xA8; size++;
        } else if (*sr1 == reg_ss) {
            op1 = 0x16;
        }
        return size;
    }
    return -1;
}

void Push::writeOperandsToStream(OStream & stream) {
    if (immSize) {
        writeImmediateToStream(stream, o1);
    }
    if (Indirect *i1 = o1->as<Indirect>(indirect)) {
        writeIndirectToStream(stream, i1, 6);
    }
}
