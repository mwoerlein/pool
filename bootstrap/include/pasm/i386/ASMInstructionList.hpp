#ifndef PASM_I386_INSTRUCTIONLIST_HPP_LOCK
#define PASM_I386_INSTRUCTIONLIST_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/String.hpp"
#include "sys/collection/HashMap.hpp"
#include "pasm/i386/ASMTypes.hpp"
#include "pasm/i386/ParseErrorStream.hpp"

class ASMOperand;
class Numeric;
class ASMInstruction;
class ASMInstructionList: virtual public Object {
    private:
    class _Elem;
    
    size_t pos;
    _Elem *first, *last;
    HashMap<String, _Elem> &ids; 
    
    public:
    OStream &warn;
    ParseErrorStream &err;
    
    ASMInstructionList(Environment &env, MemoryInfo &mi, OStream &error, bool silent = false);
    virtual ~ASMInstructionList();
    
    virtual void setMode(BitWidth mode);
    virtual void addLabel(String &label);
    virtual void addDefinition(String &definition, Numeric &value, bool global = false);
    virtual void addInstruction(ASMInstruction &inst, BitWidth data = bit_auto, BitWidth addr = bit_auto);
    
    virtual bool hasLabel(String &label);
    virtual bool hasDefinition(String &label);
    virtual bool isConstantDefinition(String &label);
    virtual size_t getLabel(String &label);
    virtual int getValue(String &label);
    virtual Numeric & getNumeric(String &label);

    virtual size_t compile();
    virtual void finalize(size_t startAddress);
    virtual bool hasErrors();
    
    virtual void writeToStream(OStream &stream);
    virtual void logToStream(OStream &stream, bool debug = false);
    virtual void logGlobalsToStream(OStream &stream);
};

#include "pasm/i386/ASMOperand.hpp"
#include "pasm/i386/Operand/Numeric.hpp"
#include "pasm/i386/ASMInstruction.hpp"

#endif //PASM_I386_INSTRUCTIONLIST_HPP_LOCK
