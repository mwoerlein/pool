#include "pasm/i386/Operand/Number.hpp"

#include "pasm/i386/ASMInstructionList.hpp"

// public
Number::Number(Environment &env, MemoryInfo &mi, int number):Numeric(env, mi), Object(env, mi), _number(number) {}
Number::~Number() {}

int Number::value() {
    return _number;
}

bool Number::isConstant(ASMInstructionList & list) {
    return true;
}

int Number::getValue(ASMInstructionList & list) {
    return _number;
}

Numeric & Number::clone() {
    return env().create<Number, int>(_number);
}

OStream & Number::operator >>(OStream & stream) {
    return stream.printuhex(_number);
}

OperandType Number::type() { return number; }
int Number::hash() {
    return _number;
}
bool Number::equals(Object &o) {
    if (ASMOperand *op = env().as<ASMOperand>(o, asm_operand)) {
        if (Number *n = op->as<Number>(number)) {
            return _number == n->_number;
        }
    }
    return false;
}
