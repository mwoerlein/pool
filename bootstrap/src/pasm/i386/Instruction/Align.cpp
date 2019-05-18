#include "pasm/i386/Instruction/Align.hpp"

#include "pasm/i386/Operand/Number.hpp"

#define isPowerOfTwo(n) ((n) && !((n) & ((n)-1)))

// protected
size_t Align::approximateSizeInBytes() {
    if (Number *n1 = o1->as<Number>(number)) {
        int align = n1->value();
        int offset = ctx->pos % align;
        return offset ? align - offset : 0;
    }
    return 0;
}

void Align::checkOperands() {
    if (!o1) {
        list->err<<"Missing value!\n";
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

void Align::validateOperands() {
    if (Number *n1 = o1->as<Number>(number)) {
        int align = n1->value();
        if (!isPowerOfTwo(align)) {
            list->err<<"alignment "<<align<<" is not a power of 2\n";
        }
        return;
    }
    list->err<<"unsupported operands in \""<<*this<<"\"\n";
}

size_t Align::compileOperands() {
    if (Number *n1 = o1->as<Number>(number)) {
        int align = n1->value();
        int offset = ctx->pos % align;
        immSize = offset ? align - offset : 0;
        return immSize;
    }
    return -1;
}

void Align::writeToStream(OStream & stream) {
    for (size_t i = 0; i < immSize; i++) {
        stream << (char) 0x90;
    }
}

void Align::writeOperandsToStream(OStream & stream) {
}

OStream & Align::operator >>(OStream & stream) {
    return stream << ".align " << *o1;
}
