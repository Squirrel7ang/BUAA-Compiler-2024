#ifndef _LEXER_HPP
#define _LEXER_HPP

#include <string>
#include <vector>
#include "token.hpp"

namespace expr {
    class Lexer {
        std::string str;

    public:
        explicit Lexer(std::string str) : str(str) {}
        Token now();
        void move();

    };
}

#endif // _LEXER_HPP