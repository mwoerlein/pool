#include "pasm/i386/Instruction/In.hpp"

// protected
size_t In::approximateSizeInBytes() {
    size_t size = 1; //opcode
    if (requiresOperandSizeOverride()) { size++; }
    if (o1->asNumeric()) { size += (int) bit_8; }
    return size;
}

void In::checkOperands() {
    if (!o1) {
        list->err<<"Missing port!\n";
    }
    if (!o2) {
        list->err<<"Missing destination!\n";
    }
    if (o3) {
        list->err<<"Unexpected operand: " << *o3 << '\n';
    }
    if (list->hasErrors()) return;
    
    Register *r2 = o2->as<Register>(reg);
    if (operandSize == bit_auto) {
        if (r2) {
            operandSize = r2->getOperandSize();
        } else {
            list->err<<"missing operand size and destination register to determine operand size from in \""<<*this<<"\"\n";
        }
    }
}

void In::validateOperands() {
    Numeric *num1 = o1->asNumeric();
    Register *r1 = o1->as<Register>(reg);
    Register *r2 = o2->as<Register>(reg);
    
    if (num1 && !(num1->isConstant(*list) && (getUnsignedBitWidth(num1->getValue(*list)) == bit_8))) {
        list->err<<"Invalid port: " << *o1 << '\n';
    }
    if (r1 && (*r1 != reg_dx)) {
        list->err<<"Invalid port register '" << *o1 << "'. Only %dx is allowed.\n";
    }
    if (r2) {
        if ((r2->kind() != reg_general) || (r2->getOpCodeRegister() != 0 /*al, ax, eax*/)) {
            list->err<<"Invalid destination register '" << *o2 << "'. Only one of %al/%ax/%eax is allowed.\n";
        }
        r2->validate(*list, operandSize);
    }
    if (list->hasErrors()) return;
    
    if ((num1 || r1) && r2) {
        return;
    }
    list->err<<"unsupported operands in \""<<*this<<"\"\n";
}

size_t In::compileOperands() {
    size_t size = 0;
    Numeric *num1 = o1->asNumeric();
    Register *r1 = o1->as<Register>(reg);
    Register *r2 = o2->as<Register>(reg);
    
    if (requiresOperandSizeOverride()) {
        pre3 = 0x66; size++;
    }
    
    if (num1 && r2) {
        immSize = (int) bit_8;
        op1 = (operandSize == bit_8) ? 0xE4 : 0xE5;
        return size + 1 + immSize;
    }
    if (r1 && r2) {
        op1 = (operandSize == bit_8) ? 0xEC : 0xED;
        return size + 1;
    }
    return -1;
}

void In::writeOperandsToStream(OStream & stream) {
    if (immSize) {
        writeImmediateToStream(stream, o1);
    }
}
