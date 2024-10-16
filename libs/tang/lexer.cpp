#include "lexer.hpp"

#include <iostream>

#include "token.hpp"
#include <istream>

namespace tang {
    Lexer::Lexer(std::istream& input, std::string& filename,
                 std::ostream& errOutput, std::ostream& correctOutput):
            _input(input),
            filename(filename),
            _errOutput(_errOutput),
            _correctOutput(correctOutput) {
        tokenNum = 0;
        curPtr = 0;
        lexPtr = 0;
        lin = 1;
        col = 1;
        ifPrint = true;
        isEnd = false;
        _lexOneToken();
    }

    char Lexer::getCh() {
        const char ch = _input.get();
        if (ch == '\n') {
            lin++;
            col = 1;
        }
        else {
            col++;
        }
        return ch;
    }

    void Lexer::consumeCh() {
        getCh();
    }

    char Lexer::peekCh() {
        return _input.peek();
    }

    int Lexer::_readASCII(std::string& content) {
        char ch = peekCh();
        if (ch == '\'' || ch == '\"') {
            return 0;
        }
        else if (ch == '\\') {
            content.append(1, '\\');
            consumeCh();
            ch = peekCh();
            switch (ch) {
            case 'a':
                // \0x07
                content.append(1, 'a');
                break;
            case 'b':
                // \0x08
                content.append(1, 'b');
                break;
            case 't':
                // \0x09
                content.append(1, 't');
                break;
            case 'n':
                // \0x0a
                content.append(1, 'n');
                break;
            case 'v':
                // \0xb
                content.append(1, 'v');
                break;
            case '\'':
                // \0xc
                content.append(1, '\'');
                break;
            case '\"':
                // \0xc
                content.append(1, '\"');
                break;
            case 'f':
                // \0xc
                content.append(1, 'f');
                break;
            case '\\':
                // \0x5c
                content.append(1, '\\');
                break;
            case '0':
                // \0
                content.append(1, '0');
                break;
            default:
                perror("no such escape character");
            }
            consumeCh();
        }
        else if (32 <= ch && ch <= 126) {
            content.append(1, ch);
            consumeCh();
        }
        return 1;
    }

    bool Lexer::skipSpace() {
        char ch = peekCh();
        bool flag = false;
        // skip empty space
        while (ch == '\n' || ch == ' ' || ch == '\t') {
            flag = true;
            consumeCh();
            ch = peekCh();
        }
        return flag;
    }

    Token Lexer::_readNextToken() {
        std::string content = "";

        skipSpace();

        char ch = getCh();
        if (ch == EOF) {
            return Token("", lin, col, TK_EOF);
        }

        // try comment token
        if (ch == '/') {
            ch = peekCh();
            if (ch == '/') {
                consumeCh();
                ch = getCh();
                content += "//";
                content += ch;
                while (ch != '\n' && ch != EOF) {
                    ch = getCh();
                    content += ch;
                }
                return Token(content, lin, col, TK_COMMENT);
            }
            else if (ch == '*') {
                char ch1 = '/';
                content += ch1;
                while (ch1 != '*' || ch != '/') {
                    content += ch;
                    ch1 = ch;
                    ch = getCh();
                    if (ch == EOF)
                        break;
                }
                content += ch;
                return Token(content, lin, col, TK_COMMENT);
            }
            else {
                ch = '/';
            }
        }

        if (std::isdigit(ch)) {
            // this is an integer constant
            int value = ch - '0';
            content.append(1, ch);
            while (std::isdigit(ch = peekCh())) {
                value *= 10;
                value += ch - '0';
                content.append(1, ch);
                consumeCh();
            }
            return Token(content, lin, col - content.size(), TK_INTCON);
        }
        else if (std::isalpha(ch) || ch == '_') {
            // identifier
            content.append(1, ch);
            ch = peekCh();
            while (isalnum(ch) || ch == '_') {
                content.append(1, ch);
                consumeCh();
                ch = peekCh();
            }
            auto it = reservedTokens.find(content);
            TokenType type = it == reservedTokens.end() ? TK_IDENFR : it->second;
            return Token(content, lin, col - content.size(), type);
        }
        else switch (ch) {
        case '(':
            content.append(1, ch);
            return Token(content, lin, col - content.size(), TK_LPARENT);
        case ')':
            content.append(1, ch);
            return Token(content, lin, col - content.size(), TK_RPARENT);
        case '[':
            content.append(1, ch);
            return Token(content, lin, col - content.size(), TK_LBRACK);
        case ']':
            content.append(1, ch);
            return Token(content, lin, col - content.size(), TK_RBRACK);
        case '{':
            content.append(1, ch);
            return Token(content, lin, col - content.size(), TK_LBRACE);
        case '}':
            content.append(1, ch);
            return Token(content, lin, col - content.size(), TK_RBRACE);
        case '+':
            content.append(1, ch);
            return Token(content, lin, col - content.size(), TK_PLUS);
        case '-':
            content.append(1, ch);
            return Token(content, lin, col - content.size(), TK_MINU);
        case '*':
            content.append(1, ch);
            return Token(content, lin, col - content.size(), TK_MULT);
        case '/':
            content.append(1, ch);
            return Token(content, lin, col - content.size(), TK_DIV);
        case '%':
            content.append(1, ch);
            return Token(content, lin, col - content.size(), TK_MOD);
        case '&': // TODO
            content.append(1, ch);
            ch = peekCh();
            if (ch == '&') {
                consumeCh();
                content.append(1, ch);
                return Token(content, lin, col - content.size(), TK_AND);
            }
            else {
                return Token("&", lin, col - content.size(), TK_UNKNOWN);
            }
        case '|': // TODO
            content.append(1, ch);
            ch = peekCh();
            if (ch == '|') {
                consumeCh();
                content.append(1, ch);
                return Token(content, lin, col - content.size(), TK_OR);
            }
            else {
                return Token("&", lin, col - content.size(), TK_UNKNOWN);
            }
        case ',':
            content.append(1, ch);
            return Token(content, lin, col - content.size(), TK_COMMA);
        case ';':
            content.append(1, ch);
            return Token(content, lin, col - content.size(), TK_SEMICN);
        case '!':
            content.append(1, ch);
            ch = peekCh();
            if (ch == '=') {
                consumeCh();
                content.append(1, ch);
                return Token(content, lin, col - content.size(), TK_NEQ);
            }
            else {
                return Token(content, lin, col - content.size(), TK_NOT);
            }
        case '=':
            content.append(1, ch);
            ch = peekCh();
            if (ch == '=') {
                consumeCh();
                content.append(1, ch);
                return Token(content, lin, col - content.size(), TK_EQL);
            }
            else {
                return Token(content, lin, col - content.size(), TK_ASSIGN);
            }
        case '<':
            content.append(1, ch);
            ch = peekCh();
            if (ch == '=') {
                consumeCh();
                content.append(1, ch);
                return Token(content, lin, col - content.size(), TK_LEQ);
            }
            else {
                return Token(content, lin, col - content.size(), TK_LSS);
            }
        case '>':
            content.append(1, ch);
            ch = peekCh();
            if (ch == '=') {
                consumeCh();
                content.append(1, ch);
                return Token(content, lin, col - content.size(), TK_GEQ);
            }
            else {
                return Token(content, lin, col - content.size(), TK_GRE);
            }
        case '\'':
            content.append(1, ch);
            _readASCII(content);
            ch = getCh();
            if (ch == '\'') {
                content.append(1, ch);
                return Token(content, lin, col - content.size(), TK_CHRCON);
            }
            else {
                perror("CHRCON not close");
                break;
            }
        case '\"':
            content.append(1, ch);
            while (_readASCII(content) == 1)
                ;
            ch = getCh();
            if (ch == '\"') {
                content.append(1, ch);
                return Token(content, lin, col - content.size(), TK_STRCON);
            }
            else {
                perror("STRCONTK not close");
                break;
            }
        case EOF:
            return Token(content, lin, col - content.size(), TK_EOF);
        default:
            char s[100];
            std::sprintf(s, "unknown token `%c`(ASCII:%u), lin: %u, col: %u", ch, ch, lin, col);
            perror(s);
            break;
        }
        return Token(content, lin, col - content.size(), TK_UNKNOWN);
    }

    Token Lexer::peekToken() {
        return peekToken(0);
    }

    void Lexer::reverse(const unsigned int n) {
        if (n > curPtr) {
            perror("reverse too much in parser");
        }
        curPtr -= n;
    }

    Token Lexer::peekToken(const unsigned int n) {
        return _peekToken(curPtr + n);
    }

    Token Lexer::_peekToken(const unsigned int index) {
        if (index >= lexPtr && isEnd) {
            // if is end, then don't lex any more
            return tokens[tokens.size() - 1];
        }
        while (index >= lexPtr)
            _lexOneToken();
        return tokens[index];
    }

    Token Lexer::getToken() {
        curPtr++;
        return peekToken(0);
    }

    void Lexer::skipToken(const unsigned int n) {
        curPtr += n;
    }


    Token Lexer::curToken() {
        return peekToken(0);
    }

    void Lexer::print(Token& t) {
        if (!ifPrint) {
            return;
        }
        if (!t.isEOF() && t.getType() != TK_COMMENT && !t.isUnknown()) {
            _correctOutput << t.toString() << ' ' << t.getContent() << std::endl;
        }
        else if (t.isUnknown()) {
            // TODO
        }
    }

    Token Lexer::_lexOneToken() {
        Token&& t = _readNextToken();
        while (t.getType() == TK_COMMENT) {
            t = _readNextToken();
        }
        if (!isEnd) {
            if (t.getType() != TK_EOF) {
                print(t);
            }
            tokens.push_back(t);
            lexPtr++;
        }
        else
            isEnd = true;
        return t;
    }
}
