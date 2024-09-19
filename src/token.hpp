#ifndef _TOKEN_HPP
#define _TOKEN_HPP

namespace expr {
    enum TokenType {
        ADD, SUB,
        MUL, DIV,
        NUM, 
    };

    class Token {
        TokenType type;
        int val; // ascii value if char, else int;

    public:
        explicit Token(TokenType type);
        bool equals(Token& t);
        bool equals(TokenType type);
        TokenType get_type();
    };
}

#endif _TOKEN_HPP