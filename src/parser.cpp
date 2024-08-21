#include "parser.h"
#include "token.h"
#include <memory>

namespace parser {

    void
    Parser::parse() {
        parse_sentence();
    }

    node::SentenceNode&
    Parser::parse_sentence() {
        node::SentenceNode* sn = new node::SentenceNode(parse_term());
        if (lexer.now().get_type() == token::ADD || lexer.now().get_type() == token::SUB) {
            lexer.move(); 

        }
        
    }

    node::TermNode&
    Parser::parse_term(node::Node parentNode) {
        node::TermNode* sn = new node::TermNode()
        
    }

    node::FactorNode&
    Parser::parse_factor() {
        node::Node = parse_term();
        
    }

    node::NumberNode&
    Parser::parse_number() {
        node::Node = parse_term();
        
    }

}