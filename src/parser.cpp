#include "parser.h"

namespace parser {

void
Parser::parse() {
    parse_sentence();
}

node::Node
Parser::parse_sentence() {
    node::Node = parse_term();
    
}

}