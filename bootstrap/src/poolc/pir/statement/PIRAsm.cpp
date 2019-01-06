#include "poolc/pir/statement/PIRAsm.hpp"

// public
PIRAsm::PIRAsm(Environment &env, MemoryInfo &mi, String &pasm, Map<String, PIRLocation> &in, Map<String, PIRLocation> &out)
        :PIRStatement(env, mi), Object(env, mi), pasm(pasm), in(in), out(out) {}
PIRAsm::~PIRAsm() {}
PIRAsm *PIRAsm::isAsm() { return this; }

OStream & PIRAsm::operator >>(OStream & stream) {
    stream << "ASM(<\"\n";
    stream << pasm << "\n";
    stream << "\">, {";
    {
        Iterator<String> &it = in.keys();
        while (it.hasNext()) {
            String &reg = it.next();
            reg.escapeToStream(stream);
            stream << ": " << in.get(reg);
            if (it.hasNext()) { stream << ", "; }
        }
        it.destroy();
    }
    stream << "}, {";
    {
        Iterator<String> &it = out.keys();
        while (it.hasNext()) {
            String &reg = it.next();
            reg.escapeToStream(stream);
            stream << ": " << out.get(reg);
            if (it.hasNext()) { stream << ", "; }
        }
        it.destroy();
    }
    stream << "})\n";
    return stream;
}
