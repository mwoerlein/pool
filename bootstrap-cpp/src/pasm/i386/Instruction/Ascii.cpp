#include "pasm/i386/Instruction/Ascii.hpp"

// protected
size_t Ascii::approximateSizeInBytes() {
    size_t size = value.length();
    if (terminalZero) { size++; }
    return size;
}

void Ascii::checkOperands() {
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

void Ascii::validateOperands() {
}

size_t Ascii::compileOperands() {
    return approximateSizeInBytes();
}

void Ascii::writeToStream(OStream & stream) {
    writeOperandsToStream(stream);
}

void Ascii::writeOperandsToStream(OStream & stream) {
    stream << value;
    if (terminalZero) { stream << (char) 0x0; }
}

OStream & Ascii::operator >>(OStream & stream) {
    stream << (terminalZero ? ".asciz " : ".ascii ");
    value.escapeToStream(stream);
}
