//
// Created by tang on 9/20/24.
//

#include <string>
#include "Token.hpp"

#include <cassert>
#include <sstream>
#include <utility>

namespace tang {
    Token::Token(std::string str,
        const unsigned int lin,
        const unsigned int col,
        const TokenType tokenType)
        : content(std::move(str)), _lin(lin), _col(col), _type(tokenType) {
    }

    unsigned int Token::getCol() const {
        return _col;
    }

    unsigned int Token::getLin() const {
        return _lin;
    }

    std::string Token::getContent() const {
        return content;
    }

    TokenType Token::getType() const {
        return _type;
    }

    bool Token::isEOF() const {
        return _type == TK_EOF;
    }

    bool Token::isUnknown() const {
        return _type == TK_UNKNOWN;
    }

    bool Token::isBType() const {
        return _type == TK_INTTK || _type == TK_CHARTK;
    }

    bool Token::isFuncType() const {
        return _type == TK_VOIDTK || isBType();
    }

    bool Token::isConstTK() const {
        return _type == TK_CONSTTK;
    }

    bool Token:: isUnaryOp() const {
        return _type == TK_PLUS || _type == TK_MINU || _type == TK_NOT;
    }

    bool Token::isMulExpOp() const {
        return _type == TK_MULT || _type == TK_DIV || _type == TK_MOD;
    }

    bool Token::isAddExpOp() const {
        return _type == TK_PLUS || _type == TK_MINU;
    }

    bool Token::isRelExpOp() const {
        return _type == TK_LEQ || _type == TK_LSS || _type == TK_GEQ || _type == TK_GRE;
    }

    bool Token::isEqExpOp() const {
        return _type == TK_NEQ || _type == TK_EQL;
    }

    bool Token::isComma() const {
        return _type == TK_COMMA;
    }

    bool Token::isExpFirst() const {
        return isUnaryOp() ||
               _type == TK_IDENFR ||
               _type == TK_LPARENT ||
               _type == TK_INTCON ||
               _type == TK_CHRCON;
    }

    std::string escapeToChar(const std::string& str) {
        std::string s;
        if (s.length() < 2) {
            s = str;
            return s;
        }

        if (str[0] != '\\') {
            s += '\\';
        }

        const int len = str.length();
        for (int i = 0; i < len - 1; i++) {
            char ch1 = str[i];
            char ch2 = str[i+1];
            if (ch1 == '\\') {
                switch (ch2) {
                    case 'a':
                        s += '\a';
                    case 'b':
                        s += '\b';
                    case 't':
                        s += '\t';
                    case 'n':
                        s += 'n';
                    case 'v':
                        s += '\v';
                    case 'f':
                        s += '\f';
                    case '\"':
                        s += '\"';
                    case '\'':
                        s += '\'';
                    case '\\':
                        s += '\\';
                    case '0':
                        s += '\0';
                    default: {
                        assert(0);
                    }
                }
                i++;
            }
            else {
                s += ch2;
            }
        }
        return s;
    }

    std::string Token::STRCONToString() const {
        if (_type == TK_STRCON) {
            const std::string str = escapeToChar(content);
            return str.substr(1, str.length() - 1);
        }
        assert(0);
        return "";
    }

    char Token::CHRCONToChar() const {
        if (_type == TK_CHRCON) {
            const std::string str = escapeToChar(content);
            return str[1];
        }
        assert(0);
    }

    std::string Token::toString() const {
        switch (_type) {
            case (TK_IDENFR): return {"IDENFR"};
            case (TK_INTCON): return {"INTCON"};
            case (TK_STRCON): return {"STRCON"};
            case (TK_CHRCON): return {"CHRCON"};
            case (TK_MAINTK): return {"MAINTK"};
            case (TK_CONSTTK): return {"CONSTTK"};
            case (TK_INTTK): return {"INTTK"};
            case (TK_CHARTK): return {"CHARTK"};
            case (TK_VOIDTK): return {"VOIDTK"};
            case (TK_FORTK): return {"FORTK"};
            case (TK_IFTK): return {"IFTK"};
            case (TK_ELSETK): return {"ELSETK"};
            case (TK_RETURNTK): return {"RETURNTK"};
            case (TK_BREAKTK): return {"BREAKTK"};
            case (TK_CONTINUETK): return {"CONTINUETK"};
            case (TK_NOT): return {"NOT"};
            case (TK_AND): return {"AND"};
            case (TK_OR): return {"OR"};
            case (TK_PLUS): return {"PLUS"};
            case (TK_MINU): return {"MINU"};
            case (TK_MULT): return {"MULT"};
            case (TK_DIV): return {"DIV"};
            case (TK_MOD): return {"MOD"};
            case (TK_GETINTTK): return {"GETINTTK"};
            case (TK_GETCHARTK): return {"GETCHARTK"};
            case (TK_PRINTFTK): return {"PRINTFTK"};
            case (TK_LSS): return {"LSS"};
            case (TK_LEQ): return {"LEQ"};
            case (TK_GRE): return {"GRE"};
            case (TK_GEQ): return {"GEQ"};
            case (TK_EQL): return {"EQL"};
            case (TK_NEQ): return {"NEQ"};
            case (TK_ASSIGN): return {"ASSIGN"};
            case (TK_SEMICN): return {"SEMICN"};
            case (TK_COMMA): return {"COMMA"};
            case (TK_LPARENT): return {"LPARENT"};
            case (TK_RPARENT): return {"RPARENT"};
            case (TK_LBRACK): return {"LBRACK"};
            case (TK_RBRACK): return {"RBRACK"};
            case (TK_LBRACE): return {"LBRACE"};
            case (TK_RBRACE): return {"RBRACE"};
            case (TK_EOF): return {"EOF"};
            case (TK_UNKNOWN): return {"UNKNOWN"};
            case (TK_COMMENT): return {"COMMENT"};
            default: perror("unknown error");
        }
        return "unknown token type";
    }


}
