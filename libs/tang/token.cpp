//
// Created by tang on 9/20/24.
//

#include <string>
#include "token.hpp"

namespace tang {
    Token::Token(const std::string &str,
        const unsigned int lin,
        const unsigned int col,
        const TokenType tokenType)
        : content(str), _lin(lin), _col(col), type(tokenType) {
    }

    unsigned int Token::getCol() const {
        return this->_col;
    }

    unsigned int Token::getLin() const {
        return this->_lin;
    }

    std::string Token::getContent() const {
        return this->content;
    }

    TokenType Token::getType() const {
        return this->type;
    }

    bool Token::isEOF() const {
        return type == TK_EOF;
    }

    bool Token::isUnknown() const {
        return type == TK_UNKNOWN;
    }

    bool Token::isBType() const {
        const auto tp = getType();
        return tp == TK_INTTK || tp == TK_CHARTK;
    }

    bool Token::isFuncType() const {
        const auto tp = getType();
        return tp == TK_VOIDTK || isBType();
    }

    std::string Token::toString() const {
        switch (type) {
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
