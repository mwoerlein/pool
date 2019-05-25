#include "poolc/storage/HeaderParser.hpp"

#include "sys/Digit.hpp"

#define SIZE 500

/*!max:re2c*/
/*!re2c
        re2c:flags:T = 1;
        re2c:tags:prefix = 'hp_yyt';
                
        re2c:define:YYCTYPE = char;
        re2c:define:YYCURSOR = current;
        re2c:define:YYMARKER = marker;
        re2c:define:YYCTXMARKER = ctxmarker;
        re2c:define:YYLIMIT = limit;
        re2c:yyfill:enable = 1;
        re2c:define:YYFILL = "if (!fillBuffer(@@, input)) return 0;";
        re2c:define:YYFILL:naked = 1;
        
        end         = "\x00";
        eol         = "\r"? "\n" | "\r";
        wsp         = [ \t]*;
        id          = [a-zA-Z_][a-zA-Z0-9_]*;
*/
/*!stags:re2c format = 'char *@@;'; */

// public
HeaderParser::HeaderParser(Environment &env, MemoryInfo &mi):ParseBuffer(env, mi, SIZE, YYMAXFILL), Object(env, mi) {}
HeaderParser::~HeaderParser() {}

size_t HeaderParser::parse(IStream & input, PropertyContainer & result) {
    char *b1, *b2, *o1, *o2, *o3, *o4;
    resetBuffer();
    
    if (!fillBuffer(3, input) || (*current++ != '/') || (*current++ != '*') || (*current != '[')) {
        // no parsable header found
        return 0;
    }
    
    for (;;) {
        token = current;
/*!re2c
        "[" wsp @b1 id @b2 wsp "]" wsp eol { continue; }
        ";" [^\r\n]* wsp eol { continue; }
        @o1 id @o2 wsp "=" wsp @o3 [^\r\n]* @o4 wsp eol {
                    String &key = result.createOwn<String, char*, char*>(b1, b2);
                    key << '.';
                    for (char *cur = o1; cur < o2; cur++) {
                        key << *cur;
                    }
                    String &value = result.createOwn<String, char*, char*>(o3, o4);
                    result.setStringProperty(key, value);
                    continue;
                  }
        "*" "/" wsp eol  { return getPos(current); }
        wsp       { continue; }
        end       { break; }
        *         { break; }
*/    
    }
    env().err() << "unexpected character : " << *token
        << " line: " << getLine(token)
        << " column: "  << getColumn(token)
        << '\n';
    return -1;
}

void HeaderParser::shift(size_t freed) {
    /*!stags:re2c format = "@@ -= freed;\n"; */
}
