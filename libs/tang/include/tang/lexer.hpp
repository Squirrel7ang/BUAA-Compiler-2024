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
        std::vector<Token> tokens;
        unsigned int tokenNum;
        int curPtr;

        std::string& filename;
        unsigned int lin;
        unsigned int col;

        bool skipSpace();
        char getCh();
        void consumeCh();
        char peekCh();
        Token _readNextToken();

    public:
        explicit Lexer(std::istream& input, std::string& filename);
        Token nextToken();
        int readASCII(std::string& content);
        // void move(); // move the pointer
        // Token now(); // return Token on current pointer
        // Token peek(unsigned int n); // peek the next n th token
    };
}

#endif //LEXER_HPP
