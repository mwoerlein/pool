#include "poolc/pir/statement/PIRGet.hpp"

// public
PIRGet::PIRGet(Environment &env, MemoryInfo &mi, PIRLocation &context, VariableScope &variable, PIRLocation &dest)
        :PIRStatement(env, mi), Object(env, mi), context(context), variable(variable), dest(dest) {}
PIRGet::~PIRGet() {}
PIRGet *PIRGet::isGet() { return this; }

OStream & PIRGet::operator >>(OStream & stream) {
    return stream << "GET(" << context << ", '" << variable.getVariableDeclNode()->name << "', " << dest << ")";
}
