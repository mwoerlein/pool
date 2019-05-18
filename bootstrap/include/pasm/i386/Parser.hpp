#ifndef PASM_I386_PARSER_HPP_LOCK
#define PASM_I386_PARSER_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/stream/IStream.hpp"

#include "pasm/i386/ParseErrorStream.hpp"
#include "pasm/i386/ASMInstructionList.hpp"
#include "pasm/i386/ASMInstruction.hpp"
#include "pasm/i386/ASMOperand.hpp"
#include "pasm/i386/Operand/Formula.hpp"
#include "pasm/i386/Operand/Number.hpp"
#include "pasm/i386/Operand/Identifier.hpp"
#include "pasm/i386/Operand/Register.hpp"

class Parser: virtual public Object {
    private:
    ASMInstructionList * list;
    MemoryInfo & buffersInfo;
    char *buffer;
    char *limit;
    char *token;
    char *current;
    char *marker;
    char *ctxmarker;
    
    int *linesBuffer;
    int *columnsBuffer;
    int currentLine;
    int currentColumn;
    
    bool freeBuffer(size_t need);
    bool fillBuffer(size_t need, IStream & input);
    
    protected:
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
    
    virtual String * parseString(IStream & input, char enclosure);
    public:
    Parser(Environment &env, MemoryInfo &mi);
    virtual ~Parser();
    
    virtual ASMInstructionList & parse(IStream & input, OStream & error, int line = 1, int column = 1, bool silent = false);
};

#endif //PASM_I386_PARSER_HPP_LOCK
