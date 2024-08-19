#include "include/token.h"

namespace token {

inline int
Token::get_lin() {
    return this->lin;
}

inline int
Token::get_col() {
    return this->col;
}

inline token_type
Token::get_type() {
    return this->type;
}

inline void
Token::set_type(token::token_type new_type) {
    this->type = new_type;
}

inline int
NumberToken::get_val() {
    return this->val;
}

inline char
SymbolToken::get_ch() {
    return this->ch;
}

inline token_type
SymbolToken::char_to_token(char ch) {
    switch (ch)
    {
    case '+':
        return token_type::ADD;
        break;
    case '-':
        return token_type::SUB;
        break;
    case '*':
        return token_type::MUL;
        break;
    case '/':
        return token_type::DIV;
        break;
    case '^':
        return token_type::EXP;
        break;
    case '=':
        return token_type::EQU;
        break;
    default:
        throw "invalid symbol";
        break;
    }
}

}