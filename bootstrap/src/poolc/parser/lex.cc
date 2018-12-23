/* Generated by re2c 1.0.1 on Wed Dec 12 20:46:26 2018 */
#include "Parser.ih"

#include "poolc/parser/PoolParser.hpp"

int Parser::makeColonEqual() { return COLON_EQUAL; }
int Parser::makeDot()        { return DOT; }
int Parser::makeEqual()      { return EQUAL; }

int Parser::makeAbstract()  { return ABSTRACT; }
int Parser::makeAs()        { return AS; }
int Parser::makeClass()     { return CLASS; }
int Parser::makeExtends()   { return EXTENDS; }
int Parser::makeGlobal()    { return GLOBAL; }
int Parser::makeNamespace() { return NAMESPACE; }
int Parser::makeReturn()    { return RETURN; }
int Parser::makeThis()      { return THIS; }
int Parser::makeUse()       { return USE; }

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
