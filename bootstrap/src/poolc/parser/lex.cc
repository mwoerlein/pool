#include "Parser.ih"

#include "poolc/parser/PoolParser.hpp"

int Parser::lex()
{
    switch (state++) {
        case 0:
            return CLASS;
        case 1:
            d_val__.u_identifier = &driver.env().create<String, const char *>("Test");
            return ID;
        case 2:
            return '{';
        case 3:
            d_val__.u_identifier = &driver.env().create<String, const char *>("dummy");
            return ID;
        case 4:
            return '(';
        case 5:
            return ')';
        case 6:
            return '{';
        case 7:
            return '}';
        case 8:
            return '}';
        default:
            return 0;
    }
}

void Parser::error()
{
    driver.error("Syntax error");
}

void Parser::exceptionHandler(std::exception const &exc)         
{
    if (driver.error(exc.what())) throw;
}
