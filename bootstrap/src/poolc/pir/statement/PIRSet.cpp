#include "poolc/pir/statement/PIRSet.hpp"

// public
PIRSet::PIRSet(Environment &env, MemoryInfo &mi, PIRLocation &context, VariableScope &variable, PIRLocation &src)
        :PIRStatement(env, mi), Object(env, mi), context(context), variable(variable), src(src) {}
PIRSet::~PIRSet() {}
PIRSet *PIRSet::isSet() { return this; }

OStream & PIRSet::operator >>(OStream & stream) {
    return stream << "SET(" << context << ", '" << variable.getVariableDeclNode()->name << "', " << src << ")";
}
