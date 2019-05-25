#include "pasm/i386/Instruction/Organize.hpp"

#include "pasm/i386/Operand/Number.hpp"

// protected
size_t Organize::approximateSizeInBytes() {
    if (Number *n1 = o1->as<Number>(number)) {
        return n1->value() - ctx->pos;
    }
    return 0;
}

void Organize::checkOperands() {
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

void Organize::validateOperands() {
    if (Number *n1 = o1->as<Number>(number)) {
        if (n1->value() < ctx->pos) {
            list->err<<"invalid organize '"<<n1->value()<<"' at pos "<<ctx->pos<<"\n";
        }
        return;
    }
    list->err<<"unsupported operands in \""<<*this<<"\"\n";
}

size_t Organize::compileOperands() {
    if (Number *n1 = o1->as<Number>(number)) {
        immSize = n1->value() - ctx->pos;
        return immSize;
    }
    return -1;
}

void Organize::writeToStream(OStream & stream) {
    for (size_t i = 0; i < immSize; i++) {
        stream << (char) 0x0;
    }
}

void Organize::writeOperandsToStream(OStream & stream) {
}

OStream & Organize::operator >>(OStream & stream) {
    return stream << ".org " << *o1;
}
