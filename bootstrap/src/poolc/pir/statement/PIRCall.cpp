#include "poolc/pir/statement/PIRCall.hpp"

// public
PIRCall::PIRCall(Environment &env, MemoryInfo &mi, PIRLocation &context, MethodScope &method, Collection<PIRLocation> &params, Collection<PIRLocation> &rets)
        :PIRStatement(env, mi), Object(env, mi), context(context), method(method), params(params), rets(rets) {}
PIRCall::~PIRCall() {}
PIRCall *PIRCall::isCall() { return this; }

OStream & PIRCall::operator >>(OStream & stream) {
    //return stream << "CALL";
    stream << "CALL(" << context << ", '" << method.getMethodDeclNode()->name << "', [";
    {
        Iterator<PIRLocation> &it = params.iterator();
        while (it.hasNext()) {
            stream << it.next();
            if (it.hasNext()) { stream << ", "; }
        }
        it.destroy();
    }
    stream << "], [";
    {
        Iterator<PIRLocation> &it = rets.iterator();
        while (it.hasNext()) {
            stream << it.next();
            if (it.hasNext()) { stream << ", "; }
        }
        it.destroy();
    }
    return stream << "])";
}
