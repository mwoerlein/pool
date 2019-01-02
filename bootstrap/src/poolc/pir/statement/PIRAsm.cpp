#include "poolc/pir/statement/PIRAsm.hpp"

// public
PIRAsm::PIRAsm(Environment &env, MemoryInfo &mi, String &pasm)
        :PIRStatement(env, mi), Object(env, mi), pasm(pasm) {}
PIRAsm::~PIRAsm() {}
PIRAsm *PIRAsm::isAsm() { return this; }

OStream & PIRAsm::operator >>(OStream & stream) {
    stream << "ASM(<\"\n";
    stream << pasm << "\n";
    return stream << "\">)\n";
}
