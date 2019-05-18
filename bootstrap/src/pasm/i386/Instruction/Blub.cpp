#include "pasm/i386/Instruction/Blub.hpp"

// protected
size_t Blub::approximateSizeInBytes() {
    return 1;
}

void Blub::checkOperands() {
    if (o1) {
        list->err<<"Unexpected operand: " << *o1 << '\n';
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

void Blub::validateOperands() {
    // list->err<<"Invalid operand: " << *o1 << '\n';
    return;
}

size_t Blub::compileOperands() {
    return -1;
}

void Blub::writeOperandsToStream(OStream & stream) {
}
