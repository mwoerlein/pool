#include "poolc/parser/PoolParser.hpp"

#include "./Parser.h"

// public
PoolParser::PoolParser(Environment &env, MemoryInfo &mi):Object(env, mi), cls(0) {}
PoolParser::~PoolParser() {}

ClassDefNode * PoolParser::parse(IStream &input) {
    Parser parser(*this);

    if (parser.parse()) {
        return 0;
    }
    
//    env().out()<<"success\n";
    return cls;
}

bool PoolParser::error(const char* msg) {
    env().err()<<msg<<"\n";
    return true;
}
