/* Generated by re2c 1.0.1 on Wed Dec 12 20:46:26 2018 */
#include "Parser.ih"

#include "poolc/parser/PoolParser.hpp"

int Parser::makeColon()             { return COLON; }
int Parser::makeColonEqual()        { return COLON_EQUAL; }
int Parser::makeColonColonEqual()   { return COLON_COLON_EQUAL; }
int Parser::makeDot()               { return DOT; }
int Parser::makeEqual()             { return EQUAL; }
int Parser::makeMinus()             { return MINUS; }
int Parser::makeMinusEqual()        { return MINUS_EQUAL; }
int Parser::makeMinusMinus()        { return MINUS_MINUS; }
int Parser::makePercentage()        { return PERCENTAGE; }
int Parser::makePercentageEqual()   { return PERCENTAGE_EQUAL; }
int Parser::makePlus()              { return PLUS; }
int Parser::makePlusEqual()         { return PLUS_EQUAL; }
int Parser::makePlusPlus()          { return PLUS_PLUS; }
int Parser::makeSlash()             { return SLASH; }
int Parser::makeSlashEqual()        { return SLASH_EQUAL; }
int Parser::makeStar()              { return STAR; }
int Parser::makeStarEqual()         { return STAR_EQUAL; }

int Parser::makeAmpersandAmpersand()     { return AMPERSAND_AMPERSAND; }
int Parser::makePipePipe()               { return PIPE_PIPE; }
int Parser::makeExclamationmark()        { return EXCLAMATIONMARK; }
int Parser::makeEqualEqual()             { return EQUAL_EQUAL; }
int Parser::makeExclamationmarkEqual()   { return EXCLAMATIONMARK_EQUAL; }
int Parser::makeAnglebracketopen()       { return ANGLEBRACKETOPEN; }
int Parser::makeAnglebracketopenEqual()  { return ANGLEBRACKETOPEN_EQUAL; }
int Parser::makeAnglebracketclose()      { return ANGLEBRACKETCLOSE; }
int Parser::makeAnglebracketcloseEqual() { return ANGLEBRACKETCLOSE_EQUAL; }

int Parser::makeAbstract()  { return ABSTRACT; }
int Parser::makeAs()        { return AS; }
int Parser::makeClass()     { return CLASS; }
int Parser::makeDo()        { return DO; }
int Parser::makeElse()      { return ELSE; }
int Parser::makeExtends()   { return EXTENDS; }
int Parser::makeGlobal()    { return GLOBAL; }
int Parser::makeIf()        { return IF; }
int Parser::makeNamespace() { return NAMESPACE; }
int Parser::makeNull()      { return T_NULL; }
int Parser::makeReturn()    { return RETURN; }
int Parser::makeStruct()    { return STRUCT; }
int Parser::makeThis()      { return THIS; }
int Parser::makeUse()       { return USE; }
int Parser::makeWhile()     { return WHILE; }

int Parser::makeAll()       { return ALL; }
int Parser::makeAny()       { return ANY; }
int Parser::makeCString()   { return CSTRING; }
int Parser::makeInt()       { return INT; }

int Parser::makeEntry()     { return ENTRY; }
int Parser::makeNaked()     { return NAKED; }
int Parser::makePasm()      { return PASM; }

int Parser::makeID(String & s) {
    d_val__.u_string = &s;
    return ID;
}

int Parser::makeString(String & s) {
    d_val__.u_string = &s;
    return STRING;
}

int Parser::makeFullQualifiedName(String & s) {
    d_val__.u_string = &s;
    return FQN;
}

int Parser::makeNumber(long value) {
    d_val__.u_number = value;
    return NUMBER;
}

void Parser::setLocation(int first_line, int first_column, int last_line, int last_column) {
    d_loc__.first_line = first_line;
    d_loc__.first_column = first_column;
    d_loc__.last_line = last_line;
    d_loc__.last_column = last_column;
}

int Parser::lex()
{
    return driver.lex();
}

void Parser::error()
{
    driver.logError("Syntax error", d_loc__.first_line, d_loc__.first_column);
}

void Parser::exceptionHandler(std::exception const &exc)         
{
    if (driver.logError(exc.what())) throw;
}
