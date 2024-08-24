#ifndef _PARSER_HPP
#define _PARSER_HPP

#include "lexer.hpp"

namespace expr {
    class Parser {
        Lexer lexer;

    public:
        explicit Parser(Lexer lexer) : lexer(lexer) {}
        int get_value();
    }
}

#endif