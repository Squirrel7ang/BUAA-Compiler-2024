#ifndef _TOKEN_HPP
#define _TOKEN_HPP

namespace expr {
    class Token {
        TokenType type;
        int val; // ascii value if char, else int;

    public:
        explicit Token(TokenType type);
        bool equals(Token t);
    };

    enum TokenType {
        ADD, SUB,
        MUL, DIV,
        NUM, 
        EQU, 
    };
}

#endif _TOKEN_HPP