#include "poolc/pir/statement/PIRMove.hpp"

// public
PIRMove::PIRMove(Environment &env, MemoryInfo &mi, PIRLocation &src, PIRLocation &dest)
        :PIRStatement(env, mi), Object(env, mi), src(src), dest(dest) {}
PIRMove::~PIRMove() {}
PIRMove *PIRMove::isMove() { return this; }

OStream & PIRMove::operator >>(OStream & stream) {
    return stream << "MOVE(" << src << ", " << dest << ")";
}
