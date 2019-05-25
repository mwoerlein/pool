#include "pasm/i386/Instruction/Out.hpp"

// protected
size_t Out::approximateSizeInBytes() {
    size_t size = 1; //opcode
    if (requiresOperandSizeOverride()) { size++; }
    if (o2->asNumeric()) { size += (int) bit_8; }
    return size;
}

void Out::checkOperands() {
    if (!o1) {
        list->err<<"Missing src!\n";
    }
    if (!o2) {
        list->err<<"Missing port!\n";
    }
    if (o3) {
        list->err<<"Unexpected operand: " << *o3 << '\n';
    }
    if (list->hasErrors()) return;
    
    Register *r1 = o1->as<Register>(reg);
    if (operandSize == bit_auto) {
        if (r1) {
            operandSize = r1->getOperandSize();
        } else {
            list->err<<"missing operand size and destination register to determine operand size from in \""<<*this<<"\"\n";
        }
    }
}

void Out::validateOperands() {
    Register *r1 = o1->as<Register>(reg);
    Register *r2 = o2->as<Register>(reg);
    Numeric *num2 = o2->asNumeric();
    
    if (r1) {
        if ((r1->kind() != reg_general) || (r1->getOpCodeRegister() != 0 /*al, ax, eax*/)) {
            list->err<<"Invalid src register '" << *o2 << "'. Only one of %al/%ax/%eax is allowed.\n";
        }
        r1->validate(*list, operandSize);
    }
    if (num2 && !(num2->isConstant(*list) && (getUnsignedBitWidth(num2->getValue(*list)) == bit_8))) {
        list->err<<"Invalid port: " << *o1 << '\n';
    }
    if (r2 && (*r2 != reg_dx)) {
        list->err<<"Invalid port register '" << *o1 << "'. Only %dx is allowed.\n";
    }
    if (list->hasErrors()) return;
    
    if (r1 && (num2 || r2)) {
        return;
    }
    list->err<<"unsupported operands in \""<<*this<<"\"\n";
}

size_t Out::compileOperands() {
    size_t size = 0;
    Register *r1 = o1->as<Register>(reg);
    Register *r2 = o2->as<Register>(reg);
    Numeric *num2 = o2->asNumeric();
    
    if (requiresOperandSizeOverride()) {
        pre3 = 0x66; size++;
    }
    
    if (r1 && num2) {
        immSize = (int) bit_8;
        op1 = (operandSize == bit_8) ? 0xE6 : 0xE7;
        return size + 1 + immSize;
    }
    if (r1 && r2) {
        op1 = (operandSize == bit_8) ? 0xEE : 0xEF;
        return size + 1;
    }
    return -1;
}

void Out::writeOperandsToStream(OStream & stream) {
    if (immSize) {
        writeImmediateToStream(stream, o2);
    }
}
