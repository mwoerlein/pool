#include "pasm/i386/Instruction/Inline.hpp"

// protected
size_t Inline::approximateSizeInBytes() {
    return (int) operandSize;
}

void Inline::checkOperands() {
    if (!o1) {
        list->err<<"Missing value!\n";
    }
    if (o2) {
        list->err<<"Unexpected operand: " << *o2 << '\n';
    }
    if (o3) {
        list->err<<"Unexpected operand: " << *o3 << '\n';
    }
    if (operandSize == bit_auto) {
        list->err<<"Missing operand size!\n";
    }
}

void Inline::validateOperands() {
    if (Numeric *num1 = o1->asNumeric()) { 
        if (num1->isConstant(*list)) {
            int value = num1->getValue(*list);
            if ((int) getUnsignedBitWidth(value) > (int) operandSize) {
                list->err<<"value '"<<value<<"' does not match "<<(operandSize*8)<<" bit operand size\n";
            }
        } else {
            if ((int) ctx->addr > (int) operandSize) {
                list->warn<<"address size "<<(ctx->addr*8)<<" bit does not match "<<(operandSize*8)<<" bit operand size: " << *this <<"\n";
            }
        }
        return;
    }
    list->err<<"unsupported operands in \""<<*this<<"\"\n";
}

size_t Inline::compileOperands() {
    if (Numeric *num1 = o1->asNumeric()) {
        immSize = (int) operandSize;
        return immSize;
    }
    return -1;
}

void Inline::writeToStream(OStream & stream) {
    writeOperandsToStream(stream);
}

void Inline::writeOperandsToStream(OStream & stream) {
    writeImmediateToStream(stream, o1);
}

OStream & Inline::operator >>(OStream & stream) {
    switch (operandSize) {
        case bit_8:  stream << ".byte "; break;
        case bit_16: stream << ".word "; break;
        case bit_32: stream << ".long "; break;
    }
    return stream << *o1;
}
