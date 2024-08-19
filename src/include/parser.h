#ifndef _PARSER_H
#define _PARSER_H

#include "lexer.h"

namespace parser {

class Parser {
    lexer::Lexer lexer;

public:
    explicit Parser(lexer::Lexer& lexer) :
        lexer(lexer) {}
    void parse();
}

}

#endif