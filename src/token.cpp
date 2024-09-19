#include "token.hpp"

namespace expr {
    bool Token::equals(Token& t) {
        return t.get_type() == this->get_type();
    }
    
    bool Token::equals(TokenType type) {
        return this->get_type() == type;
    }

    TokenType Token::get_type() {
        return this->type;
    }
}