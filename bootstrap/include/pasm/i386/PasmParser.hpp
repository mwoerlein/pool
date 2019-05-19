#ifndef PASM_I386_PASMPARSER_HPP_LOCK
#define PASM_I386_PASMPARSER_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/re2c/ParseBuffer.hpp"
#include "sys/stream/IStream.hpp"

#include "pasm/i386/ParseErrorStream.hpp"
#include "pasm/i386/ASMInstructionList.hpp"
#include "pasm/i386/ASMInstruction.hpp"
#include "pasm/i386/ASMOperand.hpp"
#include "pasm/i386/Operand/Formula.hpp"
#include "pasm/i386/Operand/Number.hpp"
#include "pasm/i386/Operand/Identifier.hpp"
#include "pasm/i386/Operand/Register.hpp"

class PasmParser: public ParseBuffer {
    private:
    ASMInstructionList * list;
    
    protected:
    virtual void shift(size_t freed) override;
    
    virtual String * readString(IStream & input, char enclosure);
    
    virtual String & parseStringValue(char * start, char * end);
    virtual int parseIntegerValue(char * start, char * end, int base = 10);
    virtual BitWidth parseOperandSize(char * start, char * end, BitWidth defaultWidth = bit_auto);
    virtual Number * parseNumber(char * start, char * end);
    virtual Formula * parseFormula(char * start, char * end);
    virtual InstructionCondition parseInstructionCondition(char * start, char * end);
    virtual Register * parseRegister(char * start, char * end);
    virtual Identifier * parseIdentifier(char * start, char * end);
    virtual ASMOperand * parseOperand(char * start, char * end);
    virtual Numeric * parseNumericOperand(char * start, char * end);
    virtual ASMInstruction * parseInstruction(char * start, char * end, char * operandsEnd, ASMOperand *op1 = 0, ASMOperand *op2 = 0, ASMOperand *op3 = 0);
    
    public:
    PasmParser(Environment &env, MemoryInfo &mi);
    virtual ~PasmParser();
    
    virtual ASMInstructionList & parse(IStream & input, OStream & error, int line = 1, int column = 1, bool silent = false);
};

#endif //PASM_I386_PASMPARSER_HPP_LOCK
