#include "lexer.h"
#include <algorithm>

namespace lexer {

Lexer::Lexer(std::string_view& str) :
    current_ptr(str),
    token_ptr(* new token::NumberToken(0, 0, 0)) { }

bool
is_symbol(char ch) {
    return  ch == '+' || 
            ch == '-' || 
            ch == '*' || 
            ch == '/' || 
            ch == '^' || 
            ch == '=';
}

token::Token &
Lexer::now() {
    return this->token_ptr;
}

void
Lexer::move() {
    delete &(this->token_ptr);
    this->token_ptr = next_token();
}

token::Token &
Lexer::next_token() {
    skip_blank();
    if (std::isdigit(current_ptr[0])) {
        return Lexer::next_number();
    }
    else if (is_symbol(current_ptr[0])) {
        return Lexer::next_symbol();
    }
    else {
        throw "invalid character";
    }
}

token::Token &
Lexer::next_number() {
    if (current_ptr[0] == '0') {
        if (current_ptr[1] == 'x') { // hexadecimal
            return Lexer::next_hexadecimal();
        }
        else { // octal
            return Lexer::next_octal();
        }
    }
    else {
        // decimal
        return Lexer::next_decimal();
    }
    // std::find_if(current_ptr.begin(), current_ptr.end(), [] (char ch) {
    //     return !(std::isdigit)
    // })
}

token::Token &
Lexer::next_hexadecimal() {
    int val;
    const auto begin = current_ptr;

    auto tail = std::find_if(current_ptr.cbegin(), current_ptr.cend(), [](char ch) {
        return !(std::isdigit(ch) || std::tolower(ch) >= 'a' && std::tolower(ch) <= 'f');
    });
    size_t len = tail - begin.cbegin();
    auto num_str = begin.substr(0, len);
    this->col += len;

    val = atoi(num_str.data());
    current_ptr = tail;

    return *(new token::NumberToken(this->lin, this->col, val));
}

token::Token &
Lexer::next_octal() {
    int val;
    const auto begin = current_ptr;

    auto tail = std::find_if(current_ptr.cbegin(), current_ptr.cend(), [](char ch) {
        return !(ch <= '7' && ch >= '0');
    });
    size_t len = tail - begin.cbegin();
    auto num_str = begin.substr(0, len);
    this->col += len;

    val = atoi(num_str.data());
    current_ptr = tail;
    
    return *(new token::NumberToken(this->lin, this->col, val));
}

token::Token &
Lexer::next_decimal() {
    int val;
    const auto begin = current_ptr;

    auto tail = std::find_if(current_ptr.cbegin(), current_ptr.cend(), [](char ch) {
        return !std::isdigit(ch);
    });
    size_t len = tail - begin.cbegin();
    auto num_str = begin.substr(0, len);
    this->col += len;

    val = atoi(num_str.data());
    current_ptr = tail;
    
    return *(new token::NumberToken(this->lin, this->col, val));
}

token::Token &
Lexer::next_symbol() {
    char ch;
    ch = current_ptr[0];
    current_ptr = current_ptr.substr(1);
    return *(new token::SymbolToken(this->lin, this->col, ch));
}

void
Lexer::skip_blank() {
    const char *p;
    for (p = current_ptr.begin(); p < current_ptr.end() && std::isspace(*p); p++) {
        if (*p == '\n') {
            this->lin++;
            this->col = 0;
        }
        this->col++;
    }
    current_ptr = current_ptr.substr(p - current_ptr.begin());
}

} // namespace lexer