#include "pasm/i386/Instruction/Int.hpp"

#include "pasm/i386/Operand/Number.hpp"

// protected
size_t Int::approximateSizeInBytes() {
    return 2;
}

void Int::checkOperands() {
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

void Int::validateOperands() {
    if (Number *n1 = o1->as<Number>(number)) {
        if (getUnsignedBitWidth(n1->value()) != bit_8) {
            list->err<<"Invalid interrupt number: " << *o1 << '\n';
        }
        return;
    }
    list->err<<"Invalid operand: " << *o1 << '\n';
    return;
}

size_t Int::compileOperands() {
    if (Number *n1 = o1->as<Number>(number)) {
        op1 = 0xCD;
        immSize = (int) bit_8;
        return 1 + immSize;
    }
    return -1;
}

void Int::writeOperandsToStream(OStream & stream) {
    if (immSize) {
        writeImmediateToStream(stream, o1);
    }
}
