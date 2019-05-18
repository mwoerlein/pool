#ifndef PASM_I386_ASMCONTEXT_HPP_LOCK
#define PASM_I386_ASMCONTEXT_HPP_LOCK

#include "sys/Object.hpp"
#include "pasm/i386/ASMTypes.hpp"

class ASMContext: virtual public Object {
    public:
    BitWidth mode, data, addr;
    size_t prePos, pos;
    ASMContext(Environment &env, MemoryInfo &mi, BitWidth mode, BitWidth data, BitWidth addr)
        :Object(env, mi), mode(mode), data(data), addr(addr), prePos(-1), pos(-1) {}
    virtual ~ASMContext() {}
    
    inline bool requiresOperandSizeOverride(BitWidth operandSize) {
        return (operandSize == bit_16 && data == bit_32) || (operandSize == bit_32 && data == bit_16);
    }
    
    inline bool requiresAddressSizeOverride(BitWidth addressSize) {
        return (addressSize == bit_16 && mode == bit_32) || (addressSize == bit_32 && mode == bit_16);
    }
    
    inline size_t approximateOffset(size_t position, size_t posIncr = 0) {
        return position - (position > pos ? prePos : pos) - posIncr;
    }
};

#endif //PASM_I386_ASMCONTEXT_HPP_LOCK
