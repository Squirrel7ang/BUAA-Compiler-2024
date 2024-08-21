#ifndef _PARSER_H
#define _PARSER_H

#include "lexer.h"
#include "node.h"

namespace parser {

    class Parser {
        lexer::Lexer lexer;

    public:
        explicit Parser(lexer::Lexer& lexer) :
            lexer(lexer) {}
        void parse();
        node::SentenceNode& parse_sentence();
        node::TermNode& parse_term();
        node::FactorNode& parse_factor();
        node::NumberNode& parse_number();
    };

}

#endif