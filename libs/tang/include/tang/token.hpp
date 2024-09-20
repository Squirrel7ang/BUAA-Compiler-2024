//
// Created by tang on 9/20/24.
//

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <unordered_map>

namespace tang {
    enum TokenType {
        TK_UNKNOWN, // unknown token
        TK_IDENFR, // identifier
        TK_INTCON, TK_STRCON, TK_CHRCON, // const value
        TK_MAINTK, // main function
        TK_CONSTTK, // const qualifier
        TK_INTTK, TK_CHARTK, TK_VOIDTK, // type
        TK_FORTK, TK_IFTK, TK_ELSETK, TK_RETURNTK, TK_BREAKTK, TK_CONTINUETK, // special statement
        TK_NOT, TK_AND, TK_OR, // logical calculation
        TK_PLUS, TK_MINU, TK_MULT, TK_DIV, TK_MOD, // algebra
        TK_GETINTTK, TK_GETCHARTK, TK_PRINTFTK, // built-in function
        TK_LSS, TK_LEQ, TK_GRE, TK_GEQ, TK_EQL, TK_NEQ, // compare
        TK_ASSIGN, // =
        TK_SEMICN, TK_COMMA, // TK_; ,
        TK_LPARENT, TK_RPARENT, TK_LBRACK, TK_RBRACK, TK_LBRACE, TK_RBRACE, // braces
    };

    const std::unordered_map<std::string, TokenType> reserveToken {
        {"main", TK_MAINTK},
        {"const", TK_CONSTTK},
        {"int", TK_INTTK},
        {"char", TK_CHARTK},
        {"void", TK_VOIDTK},
        {"if", TK_IFTK},
        {"else", TK_ELSETK},
        {"break", TK_BREAKTK},
        {"continue", TK_CONTINUETK},
        {"for", TK_FORTK},
        {"return", TK_RETURNTK},
        {"getint", TK_GETINTTK},
        {"getchar", TK_GETCHARTK},
        {"printf", TK_PRINTFTK},
    };

    class Token {
    private:
        std::string_view content;
        std::string_view _filename;
        const unsigned int _lin;
        unsigned int _col;
        TokenType type;
    public:
        explicit Token(const std::string_view& str,
            const std::string_view& filename,
            unsigned int lin,
            unsigned int col,
            TokenType tokenType);
        [[nodiscard]] std::string_view getContent() const;
        [[nodiscard]] std::string_view getFilename() const;
        [[nodiscard]] unsigned int getCol() const;
        [[nodiscard]] unsigned int getLin() const;
        [[nodiscard]] TokenType getType() const;

    }; // class Token
} // namespace tang

#endif //TOKEN_HPP
