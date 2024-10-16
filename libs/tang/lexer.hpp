//
// Created by tang on 9/20/24.
//

#ifndef LEXER_HPP
#define LEXER_HPP

#include <istream>
#include <vector>

#include "token.hpp"

namespace tang {
    // one Lexer for one Compile Unit
    class Lexer {
        std::istream& _input;
        std::ostream& _errOutput;
        std::ostream& _correctOutput;
        std::vector<Token> tokens;
        unsigned int tokenNum;
        unsigned int curPtr; // point to the current token;
        unsigned int lexPtr; // point to the about-to-be-lexed index

        std::string& filename;
        unsigned int lin;
        unsigned int col;
        bool ifPrint;

        bool skipSpace();
        char getCh();
        void consumeCh();
        char peekCh();
        Token _peekToken(unsigned int);
        Token _readNextToken();
        Token _lexOneToken();
        int _readASCII(std::string& content);

    public:
        explicit Lexer(std::istream&, std::string&);
        explicit Lexer(std::istream&, std::string&, std::ostream&, std::ostream&);
        Token peekToken(unsigned int);
        Token peekToken();
        Token getToken();
        Token curToken();

        void print(Token& t);

        void skipToken(unsigned int);
        void reverse(unsigned int);
    };
}

#endif //LEXER_HPP
