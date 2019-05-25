#ifndef PASM_I386_COMPILER_HPP_LOCK
#define PASM_I386_COMPILER_HPP_LOCK

#include "sys/stream/IStream.hpp"
#include "sys/log/LoggerAware.hpp"

class Compiler: virtual public LoggerAware {
    public:
    Compiler(Environment &env, MemoryInfo &mi);
    virtual ~Compiler();
    
    virtual MemoryInfo * compileRaw(IStream &input, size_t startAddress = -1);
};

#endif //PASM_I386_COMPILER_HPP_LOCK
