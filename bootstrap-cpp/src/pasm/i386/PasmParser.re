#include "pasm/i386/PasmParser.hpp"

#include "sys/String.hpp"
#include "sys/Digit.hpp"
#include "memory/MemoryInfoHelper.hpp"

#include "pasm/i386/Instruction/Align.hpp"
#include "pasm/i386/Instruction/Ascii.hpp"
#include "pasm/i386/Instruction/Call.hpp"
#include "pasm/i386/Instruction/ConditionalJump.hpp"
#include "pasm/i386/Instruction/GroupOneInstruction.hpp"
#include "pasm/i386/Instruction/GroupTwoInstruction.hpp"
#include "pasm/i386/Instruction/In.hpp"
#include "pasm/i386/Instruction/Inline.hpp"
#include "pasm/i386/Instruction/Int.hpp"
#include "pasm/i386/Instruction/Jump.hpp"
#include "pasm/i386/Instruction/Move.hpp"
#include "pasm/i386/Instruction/NoOperandInstruction.hpp"
#include "pasm/i386/Instruction/Organize.hpp"
#include "pasm/i386/Instruction/Out.hpp"
#include "pasm/i386/Instruction/Pop.hpp"
#include "pasm/i386/Instruction/Push.hpp"

#include "pasm/i386/Operand/Formula.hpp"
#include "pasm/i386/Operand/Identifier.hpp"
#include "pasm/i386/Operand/Indirect.hpp"
#include "pasm/i386/Operand/Number.hpp"
#include "pasm/i386/Operand/Register.hpp"

#define SIZE 5000

/*!stags:re2c format = 'char *@@;'; */
/*!max:re2c*/
/*!re2c
        re2c:flags:T = 1;
        re2c:tags:prefix = 'pasm_yyt';
        
        re2c:define:YYCTYPE = char;
        re2c:define:YYCURSOR = current;
        re2c:define:YYMARKER = marker;
        re2c:define:YYCTXMARKER = ctxmarker;
        re2c:define:YYLIMIT = limit;
        re2c:yyfill:enable = 1;
        re2c:define:YYFILL = "if (!fillBuffer(@@, input)) break;";
        re2c:define:YYFILL:naked = 1;
        
        end         = "\x00";
        eol         = "\r"? "\n" | "\r";
        wsp         = [ \t]*;
        comma       = ",";
        semicolon   = ";";
        colon       = ":";
        assign      = ":=";

        bin         = "0"[bB][01]+;
        oct         = "0"[0-7]+;
        dec         = "0" | [1-9][0-9]*; 
        hex         = "0"[xX][0-9a-fA-F]+;
        number      = "-"? (bin | oct | dec | hex);
        
        bitwidth    = [bBwWlL];
        condition   = [nN]?(([aA]|[bB]|[gG|[lL])[eE]?|[cC]|[eE]|[oO]|[pP]|[sS]|[zZ]) | [pP]([oO]|[eE]);
        register    = "%" [a-zA-Z][a-zA-Z0-9]+;
        id          = [a-zA-Z_][a-zA-Z0-9_]+;
        formula1    = "(" wsp (id|number) wsp ("+"|"-"|"*"|"/"|"%"|">>"|"<<") wsp (id|number) wsp ")";
        formula     = "(" wsp (id|number|formula1) wsp ("+"|"-"|"*"|"/"|"%"|">>"|"<<") wsp (id|number|formula1) wsp ")";
        numeric     = id | number | formula;
        id_num      = id | number;
*/
PasmParser::PasmParser(Environment &env, MemoryInfo &mi): ParseBuffer(env, mi, SIZE, YYMAXFILL), Object(env, mi) { }
PasmParser::~PasmParser() {}

// public
ASMInstructionList & PasmParser::parse(IStream & input, OStream & error, int line, int column, bool silent) {
    list = &env().create<ASMInstructionList, OStream&, bool>(error, silent);
    resetBuffer();
    char *o1, *o2, *o3, *o4, *o5, *o6, *o7, *o8;
    
    for (;;) {
        BitWidth data = bit_auto, addr = bit_auto;
        bool global = false;
detect_instruction:
        token = current;
/*!re2c
        inst        = id | "."[bB][yY][tT][eE] | "."[bB][yY][tT][eE][tT] | "."[wW][oO][rR][dD] | "."[wW][oO][rR][dD][tT] | "."[lL][oO][nN][gG] | "."[oO][rR][gG] | "."[aA][lL][iI][gG][nN];
        operand     = register | id | number | formula
                        | "(" wsp (number | id ) wsp ")"
                        | ((number | id) wsp)? "(" (wsp register)? ( wsp comma wsp register ( wsp comma wsp (id | number) )? )? wsp ")"
                    ;
        eoinst      = semicolon | eol | end | "//" | "#" | "/*";

        end       { break; }
        eol       { continue; }
        wsp       { continue; }
        semicolon { continue; }
        ( "//" | "#" ) @o1 [^\n]* @o2 eol { continue; }
        "/*" @o1 ([^*] | ("*" [^/]))* @o2 "*""/" { continue; }

        "."[cC][oO][dD][eE]"16" wsp / eoinst { list->setMode(bit_16); continue; }
        "."[cC][oO][dD][eE]"32" wsp / eoinst { list->setMode(bit_32); continue; }
        "."[dD][aA][tT][aA]"16" wsp { data = bit_16; goto detect_instruction; }
        "."[dD][aA][tT][aA]"32" wsp { data = bit_32; goto detect_instruction; }
        "."[aA][dD][dD][rR]"16" wsp { addr = bit_16; goto detect_instruction; }
        "."[aA][dD][dD][rR]"32" wsp { addr = bit_32; goto detect_instruction; }
        "."[gG][lL][oO][bB][aA][lL] wsp { global = true; goto detect_instruction; }
        
        "."[aA][sS][cC][iI] @o1 [iIzZ] wsp @o2 ['"] {
                     // read *o1 *before* parseString, because fillBuffer inside might invalidate *o1
                    bool terminalZero = (*o1 == 'z' || *o1 == 'Z');
                    String *s = readString(input, *o2);
                    if (!s) break;
                    list->addInstruction(env().create<Ascii, String&, bool>(*s, terminalZero), data, addr);
                    continue;
                  }
 
        @o1 id @o2 wsp colon {
                    list->addLabel(parseStringValue(o1, o2));
                    continue;
                  }
        @o1 id @o2 wsp assign wsp @o3 numeric @o4 wsp / eoinst {
                    list->addDefinition(parseStringValue(o1, o2), *parseNumericOperand(o3, o4), global);
                    continue;
                  }
        @o1 inst @o2 wsp / eoinst {
                    ASMInstruction * inst = parseInstruction(o1, o2, o2);
                    if (inst) {
                        list->addInstruction(*inst, data, addr);
                    }
                    continue;
                  }
        @o1 inst @o2 wsp @o3 operand @o4 wsp / eoinst {
                    ASMInstruction * inst = parseInstruction(o1, o2, o4, parseOperand(o3, o4));
                    if (inst) {
                        list->addInstruction(*inst, data, addr);
                    }
                    continue;
                  }
        @o1 inst @o2 wsp @o3 operand @o4 wsp comma wsp @o5 operand @o6 wsp / eoinst {
                    ASMInstruction * inst = parseInstruction(o1, o2, o6, parseOperand(o3, o4), parseOperand(o5, o6));
                    if (inst) {
                        list->addInstruction(*inst, data, addr);
                    }
                    continue;
                  }
        @o1 inst @o2 wsp @o3 operand @o4 wsp comma wsp @o5 operand @o6 wsp comma wsp @o7 operand @o8 wsp / eoinst {
                    ASMInstruction * inst = parseInstruction(o1, o2, o8, parseOperand(o3, o4), parseOperand(o5, o6), parseOperand(o7, o8));
                    if (inst) {
                        list->addInstruction(*inst, data, addr);
                    }
                    continue;
                  }
        *         { 
                    list->err << "unexpected character : " << *token << " line: " << getLine(token) << " column: "  << getColumn(token) << '\n';
                    break;
                  }
*/
    }
    return *list;
}

// protected
void PasmParser::shift(size_t freed) {
    /*!stags:re2c format = "@@ -= freed;\n"; */
}

String * PasmParser::readString(IStream & input, char enclosure) {
    String &s = env().create<String>();
    int startLine = getLine(current);
    int startColumn = getColumn(current);
    for (;;) {
        token = current;
/*!re2c
        *           { break; }
        [^\r\n\\]   { if (enclosure == *token) { return &s; }; s << *token; continue; }
        "\\n"       { s << '\n'; continue; }
        "\\r"       { s << '\r'; continue; }
        "\\t"       { s << '\t'; continue; }
        "\\\\"      { s << '\\'; continue; }
        "\\'"       { s << '\''; continue; }
        "\\\""      { s << '"';  continue; }
*/
    }
    list->err << "unterminated string '" << s << "' at line: " << startLine << " column: "  << startColumn << '\n';
    s.destroy();
    return 0;
}

String & PasmParser::parseStringValue(char * start, char * end) {
    return env().create<String, char*, char*>(start, end);
}

int PasmParser::parseIntegerValue(char * start, char * end, int base) {
    int result = 0;
    Digit d(env());
    while (start < end) {
        d = *start++;
        result *= base;
        result += (int) d;
    }
    return result;
}

Number * PasmParser::parseNumber(char * start, char * end) {
    char *o1, *o2, *mark, *ctx, *cur = start;
    for (;;) {
/*!re2c
        re2c:define:YYCURSOR = cur;
        re2c:define:YYMARKER = mark;
        re2c:define:YYCTXMARKER = ctx;
        re2c:define:YYLIMIT = end;
        re2c:yyfill:enable = 0;

        "0" [bB] @o1 [01]+ @o2          { if (cur != end) break; return &env().create<Number, int>( parseIntegerValue(o1, o2,  2)); }
        "-0" [bB] @o1 [01]+ @o2         { if (cur != end) break; return &env().create<Number, int>(-parseIntegerValue(o1, o2,  2)); }
        "0" @o1 [0-7]+ @o2              { if (cur != end) break; return &env().create<Number, int>( parseIntegerValue(o1, o2,  8)); }
        "-0" @o1 [0-7]+ @o2             { if (cur != end) break; return &env().create<Number, int>(-parseIntegerValue(o1, o2,  8)); }
        @o1 [1-9][0-9]* @o2             { if (cur != end) break; return &env().create<Number, int>( parseIntegerValue(o1, o2, 10)); } 
        "-" @o1 [1-9][0-9]* @o2         { if (cur != end) break; return &env().create<Number, int>(-parseIntegerValue(o1, o2, 10)); } 
        "0"[xX] @o1 [0-9a-fA-F]+ @o2    { if (cur != end) break; return &env().create<Number, int>( parseIntegerValue(o1, o2, 16)); }
        "-0"[xX] @o1 [0-9a-fA-F]+ @o2   { if (cur != end) break; return &env().create<Number, int>(-parseIntegerValue(o1, o2, 16)); }

        * { break; }
*/
    }
    return &env().create<Number, int>(0); 
}

InstructionCondition PasmParser::parseInstructionCondition(char * start, char * end) {
    char *mark, *ctx, *cur = start;
    for (;;) {
/*!re2c
        re2c:define:YYCURSOR = cur;
        re2c:define:YYMARKER = mark;
        re2c:define:YYCTXMARKER = ctx;
        re2c:define:YYLIMIT = end;
        re2c:yyfill:enable = 0;

        [aA]          { if (cur != end) break; return cond_above; }
        [aA][eE]      { if (cur != end) break; return cond_above_or_equal; }
        [bB]          { if (cur != end) break; return cond_below; }
        [bB][eE]      { if (cur != end) break; return cond_below_or_equal; }
        [cC]          { if (cur != end) break; return cond_carry; }
        [eE]          { if (cur != end) break; return cond_equal; }
        [gG]          { if (cur != end) break; return cond_greater; }
        [gG][eE]      { if (cur != end) break; return cond_greater_or_equal; }
        [lL]          { if (cur != end) break; return cond_lesser; }
        [lL][eE]      { if (cur != end) break; return cond_lesser_or_equal; }
        [oO]          { if (cur != end) break; return cond_overflow; }
        [pP]          { if (cur != end) break; return cond_parity; }
        [sS]          { if (cur != end) break; return cond_sign; }
        [zZ]          { if (cur != end) break; return cond_zero; }
 
        [nN][aA]      { if (cur != end) break; return cond_not_above; }
        [nN][aA][eE]  { if (cur != end) break; return cond_not_above_or_equal; }
        [nN][bB]      { if (cur != end) break; return cond_not_below; }
        [nN][bB][eE]  { if (cur != end) break; return cond_not_below_or_equal; }
        [nN][cC]      { if (cur != end) break; return cond_not_carry; }
        [nN][eE]      { if (cur != end) break; return cond_not_equal; }
        [nN][gG]      { if (cur != end) break; return cond_not_greater; }
        [nN][gG][eE]  { if (cur != end) break; return cond_not_greater_or_equal; }
        [nN][lL]      { if (cur != end) break; return cond_not_lesser; }
        [nN][lL][eE]  { if (cur != end) break; return cond_not_lesser_or_equal; }
        [nN][oO]      { if (cur != end) break; return cond_not_overflow; }
        [nN][pP]      { if (cur != end) break; return cond_not_parity; }
        [nN][sS]      { if (cur != end) break; return cond_not_sign; }
        [nN][zZ]      { if (cur != end) break; return cond_not_zero; }
 
        [pP][eE]      { if (cur != end) break; return cond_parity_even; }
        [pP][oO]      { if (cur != end) break; return cond_parity_odd; }

        * { break; }
*/
    }
    
    String s(env(), *notAnInfo, start, end);
    list->err << "unknown condition '" << s << "' at line: " << getLine(start) << " column: "  << getColumn(start)<< '\n';
    return cond_above;
}

Register * PasmParser::parseRegister(char * start, char * end) {
    char *mark, *ctx, *cur = start;
    for (;;) {
/*!re2c
        re2c:define:YYCURSOR = cur;
        re2c:define:YYMARKER = mark;
        re2c:define:YYCTXMARKER = ctx;
        re2c:define:YYLIMIT = end;
        re2c:yyfill:enable = 0;

        "%"[aA][hH]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_ah); }
        "%"[bB][hH]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_bh); }
        "%"[cC][hH]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_ch); }
        "%"[dD][hH]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_dh); }
        "%"[aA][lL]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_al); }
        "%"[bB][lL]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_bl); }
        "%"[cC][lL]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_cl); }
        "%"[dD][lL]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_dl); }
        
        "%"[aA][xX]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_ax); }
        "%"[bB][xX]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_bx); }
        "%"[cC][xX]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_cx); }
        "%"[dD][xX]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_dx); }
        "%"[dD][iI]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_di); }
        "%"[sS][iI]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_si); }
        "%"[bB][pP]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_bp); }
        "%"[sS][pP]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_sp); }
        
        "%"[eE][aA][xX]      { if (cur != end) break; return &env().create<Register, RegisterName>(reg_eax); }
        "%"[eE][bB][xX]      { if (cur != end) break; return &env().create<Register, RegisterName>(reg_ebx); }
        "%"[eE][cC][xX]      { if (cur != end) break; return &env().create<Register, RegisterName>(reg_ecx); }
        "%"[eE][dD][xX]      { if (cur != end) break; return &env().create<Register, RegisterName>(reg_edx); }
        "%"[eE][dD][iI]      { if (cur != end) break; return &env().create<Register, RegisterName>(reg_edi); }
        "%"[eE][sS][iI]      { if (cur != end) break; return &env().create<Register, RegisterName>(reg_esi); }
        "%"[eE][bB][pP]      { if (cur != end) break; return &env().create<Register, RegisterName>(reg_ebp); }
        "%"[eE][sS][pP]      { if (cur != end) break; return &env().create<Register, RegisterName>(reg_esp); }
        
        "%"[cC][sS]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_cs); }
        "%"[dD][sS]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_ds); }
        "%"[eE][sS]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_es); }
        "%"[fF][sS]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_fs); }
        "%"[gG][sS]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_gs); }
        "%"[sS][sS]          { if (cur != end) break; return &env().create<Register, RegisterName>(reg_ss); }

        * { break; }
*/
    }
    
    String s(env(), *notAnInfo, start, end);
    list->err << "unknown register '" << s << "' at line: " << getLine(start) << " column: "  << getColumn(start)<< '\n';
    return 0;
}

Identifier * PasmParser::parseIdentifier(char * start, char * end) {
    return &env().create<Identifier, String&>(parseStringValue(start, end));
}

Formula * PasmParser::parseFormula(char * start, char * end) {
    char *o1, *o2, *o3, *o4, *mark, *ctx, *cur = start;
    for (;;) {
/*!re2c
        re2c:define:YYCURSOR = cur;
        re2c:define:YYMARKER = mark;
        re2c:define:YYCTXMARKER = ctx;
        re2c:define:YYLIMIT = end;
        re2c:yyfill:enable = 0;

        "(" wsp @o1 numeric @o2 wsp "+" wsp @o3 numeric @o4 wsp ")" {
            if (cur != end) break;
            return &env().create<Formula, FormulaOperation, Numeric&, Numeric&>(fop_add, *parseNumericOperand(o1, o2), *parseNumericOperand(o3, o4));
        }
        "(" wsp @o1 numeric @o2 wsp "-" wsp @o3 numeric @o4 wsp ")" {
            if (cur != end) break;
            return &env().create<Formula, FormulaOperation, Numeric&, Numeric&>(fop_sub, *parseNumericOperand(o1, o2), *parseNumericOperand(o3, o4));
        }
        "(" wsp @o1 numeric @o2 wsp "*" wsp @o3 numeric @o4 wsp ")" {
            if (cur != end) break;
            return &env().create<Formula, FormulaOperation, Numeric&, Numeric&>(fop_mul, *parseNumericOperand(o1, o2), *parseNumericOperand(o3, o4));
        }
        "(" wsp @o1 numeric @o2 wsp "/" wsp @o3 numeric @o4 wsp ")" {
            if (cur != end) break;
            return &env().create<Formula, FormulaOperation, Numeric&, Numeric&>(fop_div, *parseNumericOperand(o1, o2), *parseNumericOperand(o3, o4));
        }
        "(" wsp @o1 numeric @o2 wsp "%" wsp @o3 numeric @o4 wsp ")" {
            if (cur != end) break;
            return &env().create<Formula, FormulaOperation, Numeric&, Numeric&>(fop_mod, *parseNumericOperand(o1, o2), *parseNumericOperand(o3, o4));
        }
        "(" wsp @o1 numeric @o2 wsp ">>" wsp @o3 numeric @o4 wsp ")" {
            if (cur != end) break;
            return &env().create<Formula, FormulaOperation, Numeric&, Numeric&>(fop_shr, *parseNumericOperand(o1, o2), *parseNumericOperand(o3, o4));
        }
        "(" wsp @o1 numeric @o2 wsp "<<" wsp @o3 numeric @o4 wsp ")" {
            if (cur != end) break;
            return &env().create<Formula, FormulaOperation, Numeric&, Numeric&>(fop_shl, *parseNumericOperand(o1, o2), *parseNumericOperand(o3, o4));
        }

        * { break; }
*/
    }
    String s(env(), *notAnInfo, start, end);
    list->err << "unknown formula '" << s << "' at line: " << getLine(start) << " column: "  << getColumn(start)<< '\n';
    return 0;
}

BitWidth PasmParser::parseOperandSize(char * start, char * end, BitWidth defaultWidth) {
    if (end - start != 1) {
        return defaultWidth;
    }
    switch (*start) {
        case 'l':
        case 'L':
        case 'd':
        case 'D':
            return bit_32;
        case 'w':
        case 'W':
            return bit_16;
        case 'b':
        case 'B':
            return bit_8;
        default:
            return defaultWidth;
    }
}

ASMOperand * PasmParser::parseOperand(char * start, char * end) {
    char *o1, *o2, *o3, *o4, *o5, *o6, *o7, *o8, *mark, *ctx, *cur = start;
    for (;;) {
/*!re2c
        re2c:define:YYCURSOR = cur;
        re2c:define:YYMARKER = mark;
        re2c:define:YYCTXMARKER = ctx;
        re2c:define:YYLIMIT = end;
        re2c:yyfill:enable = 0;
        
        @o1 register @o2    { if (cur != end) break; return parseRegister(o1, o2); }
        @o1 number @o2      { if (cur != end) break; return parseNumber(o1, o2); }
        @o1 id @o2          { if (cur != end) break; return parseIdentifier(o1, o2); }
        @o1 formula @o2     { if (cur != end) break; return parseFormula(o1, o2); }
        
        @o1 id_num @o2 wsp "(" wsp @o3 register @o4 wsp comma wsp @o5 register @o6 wsp comma wsp @o7 [1248] @o8 wsp ")" {
            if (cur != end) break;
            return &env().create<Indirect, Register *, Numeric *, Register *, int>(
                parseRegister(o3, o4),
                parseNumericOperand(o1, o2),
                parseRegister(o5, o6),
                parseIntegerValue(o7, o8, 10)
            );
        }
        @o1 id_num @o2 wsp "(" wsp @o3 register @o4 wsp comma wsp @o5 register @o6 wsp ")" {
            if (cur != end) break;
            return &env().create<Indirect, Register *, Numeric *, Register *>(
                parseRegister(o3, o4),
                parseNumericOperand(o1, o2),
                parseRegister(o5, o6)
            );
        }
        @o1 id_num @o2 wsp "(" wsp @o3 register @o4 wsp ")" {
            if (cur != end) break;
            return &env().create<Indirect, Register *, Numeric *>(
                parseRegister(o3, o4),
                parseNumericOperand(o1, o2)
            );
        }
        "(" wsp @o3 register @o4 wsp comma wsp @o5 register @o6 wsp comma wsp @o7 [1248] @o8 wsp ")" {
            if (cur != end) break;
            return &env().create<Indirect, Register *, Numeric *, Register *, int>(
                parseRegister(o3, o4),
                0,
                parseRegister(o5, o6),
                parseIntegerValue(o7, o8, 10)
            );
        }
        "(" wsp @o3 register @o4 wsp comma wsp @o5 register @o6 wsp ")" {
            if (cur != end) break;
            return &env().create<Indirect, Register *, Numeric *, Register *>(
                parseRegister(o3, o4),
                0,
                parseRegister(o5, o6)
            );
        }
        "(" wsp @o3 register @o4 wsp ")" {
            if (cur != end) break;
            return &env().create<Indirect, Register *>(
                parseRegister(o3, o4)
            );
        }
        @o1 id_num @o2 wsp "(" wsp comma wsp @o5 register @o6 wsp comma wsp @o7 [1248] @o8 wsp ")" {
            if (cur != end) break;
            return &env().create<Indirect, Register *, Numeric *, Register *, int>(
                0,
                parseNumericOperand(o1, o2),
                parseRegister(o5, o6),
                parseIntegerValue(o7, o8, 10)
            );
        }
        "(" wsp comma wsp @o5 register @o6 wsp comma wsp @o7 [1248] @o8 wsp ")" {
            if (cur != end) break;
            return &env().create<Indirect, Register *, Numeric *, Register *, int>(
                0,
                0,
                parseRegister(o5, o6),
                parseIntegerValue(o7, o8, 10)
            );
        }
        @o1 id_num @o2 wsp "(" wsp comma wsp @o5 register @o6 wsp ")" {
            if (cur != end) break;
            return &env().create<Indirect, Register *, Numeric *, Register *>(
                0,
                parseNumericOperand(o1, o2),
                parseRegister(o5, o6)
            );
        }
        "(" wsp @o1 id_num @o2 wsp ")" {
            if (cur != end) break;
            return &env().create<Indirect, Register *, Numeric *>(
                0,
                parseNumericOperand(o1, o2)
            );
        }

        * { break; }
*/
    }
    
    String s(env(), *notAnInfo, start, end);
    list->err << "unknown operand '" << s << "' at line: " << getLine(start) << " column: "  << getColumn(start)<< '\n';
    return 0;
}

Numeric * PasmParser::parseNumericOperand(char * start, char * end) {
    char *o1, *o2, *o3, *o4, *o5, *o6, *o7, *o8, *mark, *ctx, *cur = start;
    for (;;) {
/*!re2c
        re2c:define:YYCURSOR = cur;
        re2c:define:YYMARKER = mark;
        re2c:define:YYCTXMARKER = ctx;
        re2c:define:YYLIMIT = end;
        re2c:yyfill:enable = 0;
        
        @o1 number @o2      { if (cur != end) break; return parseNumber(o1, o2); }
        @o1 id @o2          { if (cur != end) break; return parseIdentifier(o1, o2); }
        @o1 formula @o2     { if (cur != end) break; return parseFormula(o1, o2); }
        
        * { break; }
*/
    }
    
    String s(env(), *notAnInfo, start, end);
    list->err << "unknown numeric operand '" << s << "' at line: " << getLine(start) << " column: "  << getColumn(start)<< '\n';
    return 0;
}

ASMInstruction * PasmParser::parseInstruction(char * start, char * end, char * operandsEnd, ASMOperand *op1, ASMOperand *op2, ASMOperand *op3) {
    char *o1, *o2, *mark, *ctx, *cur = start;
    for (;;) {
/*!re2c
        re2c:define:YYCURSOR = cur;
        re2c:define:YYMARKER = mark;
        re2c:define:YYCTXMARKER = ctx;
        re2c:define:YYLIMIT = end;
        re2c:yyfill:enable = 0;

        [mM][oO][vV] @o1 bitwidth? @o2 {
            if (!op1 || !op2 || op3) return 0;
            return &env().create<Move, ASMOperand*, ASMOperand*, BitWidth> (op1, op2, parseOperandSize(o1, o2));
        }
        [aA][dD][dD] @o1 bitwidth? @o2 {
            if (!op1 || !op2 || op3) return 0;
            return &env().create<GroupOneInstruction, const char *, int, ASMOperand*, ASMOperand*, BitWidth> 
                ("add", 0, op1, op2, parseOperandSize(o1, o2));
        }
        [oO][rR] @o1 bitwidth? @o2 {
            if (!op1 || !op2 || op3) return 0;
            return &env().create<GroupOneInstruction, const char *, int, ASMOperand*, ASMOperand*, BitWidth> 
                ("or", 1, op1, op2, parseOperandSize(o1, o2));
        }
        [aA][dD][cC] @o1 bitwidth? @o2 {
            if (!op1 || !op2 || op3) return 0;
            return &env().create<GroupOneInstruction, const char *, int, ASMOperand*, ASMOperand*, BitWidth> 
                ("adc", 2, op1, op2, parseOperandSize(o1, o2));
        }
        [sS][bB][bB] @o1 bitwidth? @o2 {
            if (!op1 || !op2 || op3) return 0;
            return &env().create<GroupOneInstruction, const char *, int, ASMOperand*, ASMOperand*, BitWidth> 
                ("sbb", 3, op1, op2, parseOperandSize(o1, o2));
        }
        [aA][nN][dD] @o1 bitwidth? @o2 {
            if (!op1 || !op2 || op3) return 0;
            return &env().create<GroupOneInstruction, const char *, int, ASMOperand*, ASMOperand*, BitWidth> 
                ("and", 4, op1, op2, parseOperandSize(o1, o2));
        }
        [sS][uU][bB] @o1 bitwidth? @o2 {
            if (!op1 || !op2 || op3) return 0;
            return &env().create<GroupOneInstruction, const char *, int, ASMOperand*, ASMOperand*, BitWidth> 
                ("sub", 5, op1, op2, parseOperandSize(o1, o2));
        }
        [xX][oO][rR] @o1 bitwidth? @o2 {
            if (!op1 || !op2 || op3) return 0;
            return &env().create<GroupOneInstruction, const char *, int, ASMOperand*, ASMOperand*, BitWidth> 
                ("xor", 6, op1, op2, parseOperandSize(o1, o2));
        }
        [cC][mM][pP] @o1 bitwidth? @o2 {
            if (!op1 || !op2 || op3) return 0;
            return &env().create<GroupOneInstruction, const char *, int, ASMOperand*, ASMOperand*, BitWidth> 
                ("cmp", 7, op1, op2, parseOperandSize(o1, o2));
        }
        [nN][oO][tT] @o1 bitwidth? @o2 {
            if (!op1 || op2 || op3) return 0;
            return &env().create<GroupTwoInstruction, const char *, int, ASMOperand*, BitWidth>
                ("div", 2, op1, parseOperandSize(o1, o2));
        }
        [nN][eE][gG] @o1 bitwidth? @o2 {
            if (!op1 || op2 || op3) return 0;
            return &env().create<GroupTwoInstruction, const char *, int, ASMOperand*, BitWidth>
                ("div", 3, op1, parseOperandSize(o1, o2));
        }
        [mM][uU][lL] @o1 bitwidth? @o2 {
            if (!op1 || op2 || op3) return 0;
            return &env().create<GroupTwoInstruction, const char *, int, ASMOperand*, BitWidth>
                ("mul", 4, op1, parseOperandSize(o1, o2));
        }
        [iI][mM][uU][lL] @o1 bitwidth? @o2 {
            if (op1 && !op2 && !op3) {
                return &env().create<GroupTwoInstruction, const char *, int, ASMOperand*, BitWidth>
                    ("imul", 5, op1, parseOperandSize(o1, o2));
            }
            String s(env(), *notAnInfo, start, operandsEnd);
            list->err << "not yet supported variant of imul at line: " << getLine(start) << " column: "  << getColumn(start)<< '\n';
            return 0;
        }
        [dD][iI][vV] @o1 bitwidth? @o2 {
            if (!op1 || op2 || op3) return 0;
            return &env().create<GroupTwoInstruction, const char *, int, ASMOperand*, BitWidth>
                ("div", 6, op1, parseOperandSize(o1, o2));
        }
        [iI][dD][iI][vV] @o1 bitwidth? @o2 {
            if (!op1 || op2 || op3) return 0;
            return &env().create<GroupTwoInstruction, const char *, int, ASMOperand*, BitWidth>
                ("idiv", 7, op1, parseOperandSize(o1, o2));
        }
        [iI][nN] @o1 bitwidth? @o2 {
            if (!op1 || !op2 || op3) return 0;
            return &env().create<In, ASMOperand*, ASMOperand*, BitWidth> (op1, op2, parseOperandSize(o1, o2));
        }
        [oO][uU][tT] @o1 bitwidth? @o2 {
            if (!op1 || !op2 || op3) return 0;
            return &env().create<Out, ASMOperand*, ASMOperand*, BitWidth> (op1, op2, parseOperandSize(o1, o2));
        }
        [pP][uU][sS][hH] @o1 [wWlL]? @o2 {
            if (!op1 || op2 || op3) return 0;
            return &env().create<Push, ASMOperand*, BitWidth> (op1, parseOperandSize(o1, o2));
        }
        [pP][oO][pP] @o1 [wWlL]? @o2 {
            if (!op1 || op2 || op3) return 0;
            return &env().create<Pop, ASMOperand*, BitWidth> (op1, parseOperandSize(o1, o2));
        }
        [pP][uU][sS][hH][aA] @o1 [wWlLdD]? @o2 {
            return &env().create<NoOperandInstruction, const char *, char, char, char, BitWidth>("pusha", 0x60, 0, 0, parseOperandSize(o1, o2, bit_16));
        }
        [pP][oO][pP][aA] @o1 [wWlLdD]? @o2 {
            return &env().create<NoOperandInstruction, const char *, char, char, char, BitWidth>("popa", 0x61, 0, 0, parseOperandSize(o1, o2, bit_16));
        }
        [pP][uU][sS][hH][fF] @o1 [wWlLdD]? @o2 {
            return &env().create<NoOperandInstruction, const char *, char, char, char, BitWidth>("pushf", 0x9C, 0, 0, parseOperandSize(o1, o2, bit_16));
        }
        [pP][oO][pP][fF] @o1 [wWlLdD]? @o2 {
            return &env().create<NoOperandInstruction, const char *, char, char, char, BitWidth>("popf", 0x9D, 0, 0, parseOperandSize(o1, o2, bit_16));
        }
        [lL][oO][dD][sS][bB] {
            return &env().create<NoOperandInstruction, const char *, char, char, char, BitWidth>("lods", 0xAC, 0, 0, bit_8);
        }
        [lL][oO][dD][sS] @o1 [wWlLdD]? @o2 {
            return &env().create<NoOperandInstruction, const char *, char, char, char, BitWidth>("lods", 0xAD, 0, 0, parseOperandSize(o1, o2));
        }
        [cC][lL][cC] {
            return &env().create<NoOperandInstruction, const char *, char>("clc", 0xF8);
        }
        [cC][lL][dD] {
            return &env().create<NoOperandInstruction, const char *, char>("cld", 0xFC);
        }
        [cC][lL][iI] {
            return &env().create<NoOperandInstruction, const char *, char>("cli", 0xFA);
        }
        [cC][lL][tT][sS] {
            return &env().create<NoOperandInstruction, const char *, char, char>("clts", 0x0F, 0x06);
        }
        [sS][tT][cC] {
            return &env().create<NoOperandInstruction, const char *, char>("stc", 0xF9);
        }
        [sS][tT][dD] {
            return &env().create<NoOperandInstruction, const char *, char>("std", 0xFD);
        }
        [sS][tT][iI] {
            return &env().create<NoOperandInstruction, const char *, char>("sti", 0xFB);
        }
        [hH][lL][tT] {
            return &env().create<NoOperandInstruction, const char *, char>("hlt", 0xF4);
        }
        [nN][oO][pP] {
            return &env().create<NoOperandInstruction, const char *, char>("nop", 0x90);
        }
        [iI][nN][tT] {
            if (!op1 || op2 || op3) return 0;
            return &env().create<Int, ASMOperand*> (op1);
        }
        [iI][nN][tT]"0" {
            return &env().create<NoOperandInstruction, const char *, char>("int0", 0xCE);
        }
        [iI][nN][tT]"1" {
            return &env().create<NoOperandInstruction, const char *, char>("int1", 0xF1);
        }
        [iI][nN][tT]"3" {
            return &env().create<NoOperandInstruction, const char *, char>("int3", 0xCC);
        }
        [jJ][mM][pP] {
            if (!op1 || op2 || op3) return 0;
            return &env().create<Jump, ASMOperand*>(op1);
        }
        [cC][aA][lL][lL] {
            if (!op1 || op2 || op3) return 0;
            return &env().create<Call, ASMOperand*>(op1);
        }
        [lL][eE][aA][vV][eE] {
            if (op1 || op2 || op3) return 0;
            return &env().create<NoOperandInstruction, const char *, char>("leave", 0xC9);
        }
        [rR][eE][tT] {
            if (op1 || op2 || op3) return 0;
            return &env().create<NoOperandInstruction, const char *, char>("ret", 0xC3);
        }
        [iI][rR][eE][tT] {
            if (op1 || op2 || op3) return 0;
            return &env().create<NoOperandInstruction, const char *, char>("iret", 0xCF);
        }
        [jJ] @o1 condition @o2 {
            if (!op1 || op2 || op3) return 0;
            return &env().create<ConditionalJump, InstructionCondition, ASMOperand*>(parseInstructionCondition(o1, o2), op1);
        }
        [jJ][cC][xX][zZ] {
            if (!op1 || op2 || op3) return 0;
            return &env().create<ConditionalJump, InstructionCondition, ASMOperand*>(cond_reg_cx, op1);
        }
        [jJ][eE][cC][xX][zZ] {
            if (!op1 || op2 || op3) return 0;
            return &env().create<ConditionalJump, InstructionCondition, ASMOperand*>(cond_reg_ecx, op1);
        }

        "."[bB][yY][tT][eE] {
            if (!op1 || op2 || op3) return 0;
            return &env().create<Inline, ASMOperand*, BitWidth>(op1, bit_8);
        }
        "."[wW][oO][rR][dD] {
            if (!op1 || op2 || op3) return 0;
            return &env().create<Inline, ASMOperand*, BitWidth>(op1, bit_16);
        }
        "."[bB][yY][tT][eE][tT] {
            if (!op1 || op2 || op3) return 0;
            return &env().create<Inline, ASMOperand*, BitWidth, bool>(op1, bit_8, false);
        }
        "."[wW][oO][rR][dD][tT] {
            if (!op1 || op2 || op3) return 0;
            return &env().create<Inline, ASMOperand*, BitWidth, bool>(op1, bit_16, false);
        }
        "."[lL][oO][nN][gG] {
            if (!op1 || op2 || op3) return 0;
            return &env().create<Inline, ASMOperand*, BitWidth>(op1, bit_32);
        }
        "."[oO][rR][gG] {
            if (!op1 || op2 || op3) return 0;
            return &env().create<Organize, ASMOperand*>(op1);
        }
        "."[aA][lL][iI][gG][nN] {
            if (!op1 || op2 || op3) return 0;
            return &env().create<Align, ASMOperand*>(op1);
        }

        * { break; }
*/
    }
    
    String s(env(), *notAnInfo, start, operandsEnd);
    list->err << "unknown instruction '" << s << "' at line: " << getLine(start) << " column: "  << getColumn(start)<< '\n';
    return 0;
}
