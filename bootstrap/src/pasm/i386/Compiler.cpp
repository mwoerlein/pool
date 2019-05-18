#include "pasm/i386/Compiler.hpp"

#include "memory/MemoryIOStream.hpp"
#include "pasm/i386/Parser.hpp"
#include "pasm/i386/ASMInstructionList.hpp"

// public
Compiler::Compiler(Environment &env, MemoryInfo &mi): Object(env, mi), LoggerAware(env, mi) {}
Compiler::~Compiler() {}

//bool Compiler::compile(IStream &input, OStream &out, size_t startAddress = 0);
MemoryInfo * Compiler::compileRaw(IStream &input, size_t startAddress) {
    Parser &parser = env().create<Parser>();
    ASMInstructionList &list = parser.parse(input, env().err(), 1, 1, true); // silent
    parser.destroy();
    
    if (list.hasErrors()) {
        error()<<"parsing error\n";
        list.destroy();
        return notAnInfo;
    }
    
    size_t size = list.compile();
    if (list.hasErrors()) {
        error()<<"compile error\n";
        list.destroy();
        return notAnInfo;
    }
    
    MemoryInfo *memInfo = &env().getAllocator().allocate(size);
    
    list.finalize(startAddress == -1 ? (size_t) memInfo->buf : startAddress);
    if (list.hasErrors()) {
        error()<<"finalize error\n";
        env().getAllocator().free(*memInfo);
        list.destroy();
        return notAnInfo;
    }
    
    //if (logLevel >= 2) { list.logToStream(env().out(), logLevel >= 3); }
    
    MemoryIOStream &mem = env().create<MemoryIOStream, MemoryInfo&>(*memInfo);
    list.writeToStream(mem);
    mem.destroy();
    list.destroy();
    
    return memInfo;
}
