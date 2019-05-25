#include "poolc/pir/statement/PIRArithOp.hpp"

// public
PIRArithOp::PIRArithOp(Environment &env, MemoryInfo &mi, arith_op op, PIRLocation &left, PIRLocation &right, PIRLocation &dest)
        :PIRStatement(env, mi), Object(env, mi), op(op), left(left), right(right), dest(dest) {}
PIRArithOp::~PIRArithOp() {}
PIRArithOp *PIRArithOp::isArithOp() { return this; }

OStream & PIRArithOp::operator >>(OStream & stream) {
    stream << "ARITHOP(" << dest << " = " << left;
    switch (op) {
        case op_add: stream << " + "; break;
        case op_sub: stream << " - "; break;
        case op_mul: stream << " * "; break;
        case op_div: stream << " / "; break;
        case op_mod: stream << " % "; break;
    }
    return stream << right << ")";
}
