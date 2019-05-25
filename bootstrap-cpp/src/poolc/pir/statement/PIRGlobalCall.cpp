#include "poolc/pir/statement/PIRGlobalCall.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"

// public
PIRGlobalCall::PIRGlobalCall(Environment &env, MemoryInfo &mi, MethodScope &method, Collection<PIRLocation> &params, Collection<PIRLocation> &rets)
        :PIRStatement(env, mi), Object(env, mi), method(method), params(params), rets(rets) {}
PIRGlobalCall::~PIRGlobalCall() {}
PIRGlobalCall *PIRGlobalCall::isGlobalCall() { return this; }

OStream & PIRGlobalCall::operator >>(OStream & stream) {
    stream << "GLOBALCALL(" << method.getClassDeclNode()->fullQualifiedName << ", '" << method.getMethodDeclNode()->name << "', [";
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
