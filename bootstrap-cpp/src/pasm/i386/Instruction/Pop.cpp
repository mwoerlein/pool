#include "pasm/i386/Instruction/Pop.hpp"

// protected
size_t Pop::approximateSizeInBytes() {
    size_t size = 1; // op1
    Indirect *i1 = o1->as<Indirect>(indirect);
    Register *r1 = o1->as<Register>(reg);
    Register *sr1 = (r1 && (r1->kind() == reg_segment)) ? r1 : 0;
    
    if (requiresOperandSizeOverride()) { size++; }
    
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

void Pop::checkOperands() {
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

void Pop::validateOperands() {
    Indirect *i1 = o1->as<Indirect>(indirect);
    Register *r1 = o1->as<Register>(reg);
    
    if (r1) {
        if (r1->kind() == reg_general) {
            r1->validate(*list, operandSize);
        }
        if (*r1 == reg_cs) {
            list->err << "invalid register in \""<<*this<<"\"\n";
        }
    }
    if (list->hasErrors()) return;
    
    if (r1 || i1) {
        return;
    }
    list->err<<"unsupported operands in \""<<*this<<"\"\n";
}

size_t Pop::compileOperands() {
    size_t size = 1; // op1
    Indirect *i1 = o1->as<Indirect>(indirect);
    Register *r1 = o1->as<Register>(reg);
    Register *gr1 = (r1 && (r1->kind() == reg_general)) ? r1 : 0;
    Register *sr1 = (r1 && (r1->kind() == reg_segment)) ? r1 : 0;
    
    if (requiresOperandSizeOverride()) {
        pre3 = 0x66; size++;
    }
    
    if (gr1) {
        op1 = 0x58 + gr1->getOpCodeRegister();
        return size;
    }
    if (i1) {
        op1 = 0x8F;
        if (requiresAddressSizeOverride(i1)) {
            pre4 = 0x67; size++;
        }    
        useIndirectSizes(i1);
        return size + modrmSize + sibSize + dispSize;
    }
    if (sr1) {
        if (*sr1 == reg_ds) {
            op1 = 0x1F;
        } else if (*sr1 == reg_es) {
            op1 = 0x07;
        } else if (*sr1 == reg_fs) {
            op1 = 0x0F;
            op2 = 0xA1; size++;
        } else if (*sr1 == reg_gs) {
            op1 = 0x0F;
            op2 = 0xA9; size++;
        } else if (*sr1 == reg_ss) {
            op1 = 0x17;
        }
        return size;
    }
    return -1;
}

void Pop::writeOperandsToStream(OStream & stream) {
    if (Indirect *i1 = o1->as<Indirect>(indirect)) {
        writeIndirectToStream(stream, i1, 0);
    }
}
