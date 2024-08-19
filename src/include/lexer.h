#ifndef _LEXER_H
#define _LEXER_H

#include "token.h"
#include <vector>
#include <optional>
#include <compare>
#include <string>
#include <cctype>

namespace lexer {

class Lexer {
    std::string_view current_ptr;
    token::Token& token_ptr;
    int lin;
    int col;
    
public:
    [[nodiscard]]token::Token& now();
    void move();
    explicit Lexer(std::string_view& str);

private:
    void skip_blank();
    token::Token& next_token();
    token::Token& next_number();
    token::Token& next_hexadecimal();
    token::Token& next_octal();
    token::Token& next_decimal();
    token::Token& next_symbol();
};

}

#endif // _LEXER_H