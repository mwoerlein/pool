#include "pasm/i386/Instruction/NoOperandInstruction.hpp"

// public
NoOperandInstruction::NoOperandInstruction(Environment &env, MemoryInfo &mi, const char * mnemonic, char opCode1, char opCode2, char opCode3, BitWidth operandSize)
        :ASMInstruction(env, mi, mnemonic, operandSize), Object(env, mi) {
    op1 = opCode1;
    op2 = opCode2;
    op3 = opCode3;
}
NoOperandInstruction::~NoOperandInstruction() {}

// protected
size_t NoOperandInstruction::approximateSizeInBytes() {
    size_t size = requiresOperandSizeOverride() ? 1 : 0;
    if (op1 == 0x0F) {
        if (op2 == 0x38 || op2 == 0x3A) {
            return 3 + size;
        }
        return 2 + size;
    }
    return 1 + size;
}

void NoOperandInstruction::checkOperands() {
    if (o1) {
        list->err<<"Unexpected operand: " << *o1 << '\n';
    }
    if (o2) {
        list->err<<"Unexpected operand: " << *o2 << '\n';
    }
    if (o3) {
        list->err<<"Unexpected operand: " << *o3 << '\n';
    }
}

size_t NoOperandInstruction::compileOperands() {
    if (requiresOperandSizeOverride()) { pre3 = 0x66; }
    return approximateSizeInBytes();
}

void NoOperandInstruction::writeOperandsToStream(OStream &stream) {}
