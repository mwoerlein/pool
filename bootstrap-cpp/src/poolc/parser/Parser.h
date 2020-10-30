// Generated by Bisonc++ V6.01.00 on Sun, 16 Dec 2018 07:25:13 +0100

#ifndef Parser_h_included
#define Parser_h_included

// $insert baseclass
#include "Parserbase.h"


#undef Parser
    // CAVEAT: between the baseclass-include directive and the 
    // #undef directive in the previous line references to Parser 
    // are read as ParserBase.
    // If you need to include additional headers in this file 
    // you should do so after these comment-lines.

#include "poolc/parser/PoolParser.hpp"

class Parser: public ParserBase
{
        
    public:
        Parser(PoolParser &driver):driver(driver){};
        
        void setLocation(int first_line, int first_column, int last_line, int last_column);
        
        int makeColon();
        int makeColonEqual();
        int makeColonColonEqual();
        int makeDot();
        int makeEqual();
        int makeMinus();
        int makeMinusEqual();
        int makeMinusMinus();
        int makePercentage();
        int makePercentageEqual();
        int makePlus();
        int makePlusEqual();
        int makePlusPlus(); 
        int makeSlash();
        int makeSlashEqual();
        int makeStar();
        int makeStarEqual();
        
        int makeAmpersandAmpersand();
        int makePipePipe();
        int makeExclamationmark();
        int makeEqualEqual();
        int makeExclamationmarkEqual();
        int makeAnglebracketopen();
        int makeAnglebracketopenEqual();
        int makeAnglebracketclose();
        int makeAnglebracketcloseEqual();

        int makeAbstract();
        int makeAs();
        int makeClass();
        int makeDo();
        int makeElse();
        int makeExtends();
        int makeGlobal();
        int makeIf();
        int makeNamespace();
        int makeNull();
        int makeReturn();
        int makeStruct();
        int makeThis();
        int makeUse();
        int makeWhile();
        
        int makeAll();
        int makeAny();
        int makeCString();
        int makeInt();
        
        int makeEntry();
        int makeNaked();
        int makePasm();
        
        int makeID(String & s);
        int makeString(String & s);
        int makeFullQualifiedName(String & s);
        int makeNumber(long value);
        
        int parse();

    private:
        PoolParser &driver;

        void error();                   // called on (syntax) errors
        int lex();                      // returns the next token from the
                                        // lexical scanner. 
        void print();                   // use, e.g., d_token, d_loc
        void exceptionHandler(std::exception const &exc);

    // support functions for parse():
        void executeAction_(int ruleNr);
        void errorRecovery_();
        void nextCycle_();
        void nextToken_();
        void print_();
};


#endif
