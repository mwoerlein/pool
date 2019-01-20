#include "poolc/pir/statement/PIRCond.hpp"

// public
PIRCond::PIRCond(Environment &env, MemoryInfo &mi, logical_op op, PIRLocation &left, PIRLocation &right)
        :PIRStatement(env, mi), Object(env, mi), op(op), left(left), right(right) {}
PIRCond::~PIRCond() {}
PIRCond *PIRCond::isCond() { return this; }

OStream & PIRCond::operator >>(OStream & stream) {
    stream << "CONDOP(" << left;
    switch (op) {
        case op_eq: stream << " == "; break;
        case op_neq: stream << " != "; break;
        case op_lt: stream << " < "; break;
        case op_le: stream << " <= "; break;
        case op_gt: stream << " > "; break;
        case op_ge: stream << " >= "; break;
    }
    return stream << right << ")";
}
