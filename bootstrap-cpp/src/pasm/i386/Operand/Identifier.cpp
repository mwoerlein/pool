#include "pasm/i386/Operand/Identifier.hpp"

#include "pasm/i386/ASMInstructionList.hpp"

// public
Identifier::Identifier(Environment &env, MemoryInfo &mi, String &id):Numeric(env, mi), Object(env, mi), _id(id) {}
Identifier::~Identifier() {
    _id.destroy();
}
    
String & Identifier::id() {
    return _id;
}

OStream & Identifier::operator >>(OStream & stream) {
    return stream << _id;
}

bool Identifier::isConstant(ASMInstructionList & list) {
    return list.isConstantDefinition(_id);
}

int Identifier::getValue(ASMInstructionList & list) {
    return list.getValue(_id);
}

Numeric & Identifier::clone() {
    return env().create<Identifier, String&>(env().create<String, String&>(_id));
}

Numeric * Identifier::validateAndReplace(ASMInstructionList & list, BitWidth mode) {
    if (list.hasDefinition(_id)) {
        Numeric & num = list.getNumeric(_id);
        if (Numeric * op = num.validateAndReplace(list, mode)) {
            num.destroy();
            return op;
        }
        return &num;
    } else if (!list.hasLabel(_id)) {
        list.err << "Unknown identifier: " << *this << '\n';
    }
    return 0;
}

OperandType Identifier::type() { return identifier; }
int Identifier::hash() {
    return _id.hash();
}
bool Identifier::equals(Object &o) {
    if (ASMOperand *op = env().as<ASMOperand>(o, asm_operand)) {
        if (Identifier *id = op->as<Identifier>(identifier)) {
            return _id.equals(id->_id);
        }
    }
    return false;
}
