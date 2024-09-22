#include "tang/lexer.hpp"

#include <iostream>

#include "tang/token.hpp"
#include <istream>

namespace tang {
    Lexer::Lexer(std::istream& input, std::string& filename)
        : input(input), filename(filename) {
        tokenNum = 0;
        curPtr = 0;
        lin = 1;
        col = 1;
    }

    int Lexer::readASCII(std::string& content) {
        char ch = input.get();
        if (ch == '\'' || ch == '\"') {
            input.unget();
            return 0;
        }
        else if (ch == '\\') {
            col++;
            content.append(1, '\\');
            ch = input.get();
            switch (ch) {
            case 'a':
                // \0x07
                col++;
                content.append(1, 'a');
                break;
            case 'b':
                // \0x08
                col++;
                content.append(1, 'b');
                break;
            case 't':
                // \0x09
                col++;
                content.append(1, 't');
                break;
            case 'n':
                // \0x0a
                col++;
                content.append(1, 'n');
                break;
            case 'v':
                // \0xb
                col++;
                content.append(1, 'v');
                break;
            case '\'':
                // \0xc
                col++;
                content.append(1, '\'');
                break;
            case '\"':
                // \0xc
                col++;
                content.append(1, '\"');
                break;
            case 'f':
                // \0xc
                col++;
                content.append(1, 'f');
                break;
            case '\\':
                // \0x5c
                col++;
                content.append(1, '\\');
                break;
            case '0':
                // \0
                col++;
                content.append(1, '0');
                break;
            default:
                input.unget();
                perror("no such token");
            }
        }
        else if (32 <= ch && ch <= 126) {
            col++;
            content.append(1, ch);
        }
        return 1;
    }

    Token Lexer::readNextToken() {
        char ch = input.get();
        std::string content;
        while (ch == '\n' || ch == ' ' || ch == '\t') {
            if (ch == '\n') {
                lin++;
                col = 1;
            }
            else {
                col++;
            }
            ch = input.get();
        }

        if (std::isdigit(ch)) {
            // this is an integer constant
            int value = ch - '0';
            content.append(1, ch);
            col++;
            while (std::isdigit(ch = input.get())) {
                value *= 10;
                value += ch - '0';
                content.append(1, ch);
                col++;
            }
            input.unget();
            return Token(content, filename, lin, col - content.size(), TK_INTTK);
        }
        else if (std::isalpha(ch) || ch == '_') {
            // identifier
            content.append(1, ch);
            ch = input.get();
            col++;
            while (isalnum(ch) || ch == '_') {
                content.append(1, ch);
                ch = input.get();
                col++;
            }
            input.unget();
            auto it = reservedTokens.find(content);
            TokenType type = it == reservedTokens.end() ? TK_IDENFR : it->second;
            return Token(content, filename, lin, col - content.size(), type);
        }
        else switch (ch) {
        case '(':
            col++;
            content.append(1, ch);
            return Token(content, filename, lin, col - content.size(), TK_LPARENT);
        case ')':
            col++;
            content.append(1, ch);
            return Token(content, filename, lin, col - content.size(), TK_RPARENT);
        case '[':
            col++;
            content.append(1, ch);
            return Token(content, filename, lin, col - content.size(), TK_LBRACK);
        case ']':
            col++;
            content.append(1, ch);
            return Token(content, filename, lin, col - content.size(), TK_RBRACK);
        case '{':
            col++;
            content.append(1, ch);
            return Token(content, filename, lin, col - content.size(), TK_LBRACE);
        case '}':
            col++;
            content.append(1, ch);
            return Token(content, filename, lin, col - content.size(), TK_RBRACE);
        case '+':
            col++;
            content.append(1, ch);
            return Token(content, filename, lin, col - content.size(), TK_PLUS);
        case '-':
            col++;
            content.append(1, ch);
            return Token(content, filename, lin, col - content.size(), TK_MINU);
        case '*':
            col++;
            content.append(1, ch);
            return Token(content, filename, lin, col - content.size(), TK_MULT);
        case '/':
            col++;
            content.append(1, ch);
            return Token(content, filename, lin, col - content.size(), TK_DIV);
        case '%':
            col++;
            content.append(1, ch);
            return Token(content, filename, lin, col - content.size(), TK_MOD);
        case '&': // TODO
            col++;
            content.append(1, ch);
            ch = input.get();
            if (ch == '&') {
                col++;
                content.append(1, ch);
                return Token(content, filename, lin, col - content.size(), TK_AND);
            }
            else {
                input.unget();
                return Token("&", filename, lin, col - content.size(), TK_UNKNOWN);
                break;
            }
        case '|': // TODO
            col++;
            content.append(1, ch);
            ch = input.get();
            if (ch == '|') {
                col++;
                content.append(1, ch);
                return Token(content, filename, lin, col - content.size(), TK_AND);
            }
            else {
                input.unget();
                return Token("&", filename, lin, col - content.size(), TK_UNKNOWN);
                break;
            }
        case ',':
            col++;
            content.append(1, ch);
            return Token(content, filename, lin, col - content.size(), TK_COMMA);
        case ';':
            col++;
            content.append(1, ch);
            return Token(content, filename, lin, col - content.size(), TK_SEMICN);
        case '!':
            col++;
            content.append(1, ch);
            ch = input.get();
            if (ch == '=') {
                col++;
                content.append(1, ch);
                return Token(content, filename, lin, col - content.size(), TK_NEQ);
            }
            else {
                input.unget();
                return Token(content, filename, lin, col - content.size(), TK_NOT);
            }
        case '=':
            col++;
            content.append(1, ch);
            ch = input.get();
            if (ch == '=') {
                col++;
                content.append(1, ch);
                return Token(content, filename, lin, col - content.size(), TK_EQL);
            }
            else {
                input.unget();
                return Token(content, filename, lin, col - content.size(), TK_ASSIGN);
            }
        case '<':
            col++;
            ch = input.get();
            if (ch == '=') {
                col++;
                content.append(1, ch);
                return Token(content, filename, lin, col - content.size(), TK_LEQ);
            }
            else {
                input.unget();
                return Token(content, filename, lin, col - content.size(), TK_LSS);
            }
        case '>':
            col++;
            ch = input.get();
            if (ch == '=') {
                col++;
                content.append(1, ch);
                return Token(content, filename, lin, col - content.size(), TK_GEQ);
            }
            else {
                input.unget();
                return Token(content, filename, lin, col - content.size(), TK_GRE);
            }
        case '\'':
            col++;
            content.append(1, ch);
            readASCII(content);
            ch = input.get();
            if (ch == '\'') {
                col++;
                content.append(1, ch);
                return Token(content, filename, lin, col - content.size(), TK_CHARTK);
            }
            else {
                perror("CHARTK not close");
                break;
            }
        case '\"':
            col++;
            content.append(1, ch);
            while (readASCII(content) == 1)
                ;
            ch = input.get();
            if (ch == '\"') {
                col++;
                content.append(1, ch);
                return Token(content, filename, lin, col - content.size(), TK_STRCON);
            }
            else {
                perror("STRCONTK not close");
                break;
            }
        case EOF:
            return Token(content, filename, lin, col - content.size(), TK_EOF);
        default:
            perror("unknown token");
            break;
        }
    }

}
