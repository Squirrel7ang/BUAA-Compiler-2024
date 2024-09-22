//
// Created by tang on 9/20/24.
//

#include <string>
#include "tang/token.hpp"

namespace tang {
    Token::Token(const std::string &str,
        const std::string& filename,
        const unsigned int lin,
        const unsigned int col,
        const TokenType tokenType)
        : content(str), _filename(filename), _lin(lin), _col(col), type(tokenType) {
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

    std::string Token::getFilename() const {
        return this->_filename;
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

    std::string Token::toString() const {
        switch (type) {
            case (TK_UNKNOWN): return {"TK_UNKNOWN"};
            case (TK_IDENFR): return {"TK_IDENFR"};
            case (TK_INTCON): return {"TK_INTCON"};
            case (TK_STRCON): return {"TK_STRCON"};
            case (TK_CHRCON): return {"TK_CHRCON"};
            case (TK_MAINTK): return {"TK_MAINTK"};
            case (TK_CONSTTK): return {"TK_CONSTTK"};
            case (TK_INTTK): return {"TK_INTTK"};
            case (TK_CHARTK): return {"TK_CHARTK"};
            case (TK_VOIDTK): return {"TK_VOIDTK"};
            case (TK_FORTK): return {"TK_FORTK"};
            case (TK_IFTK): return {"TK_IFTK"};
            case (TK_ELSETK): return {"TK_ELSETK"};
            case (TK_RETURNTK): return {"TK_RETURNTK"};
            case (TK_BREAKTK): return {"TK_BREAKTK"};
            case (TK_CONTINUETK): return {"TK_CONTINUETK"};
            case (TK_NOT): return {"TK_NOT"};
            case (TK_AND): return {"TK_AND"};
            case (TK_OR): return {"TK_OR"};
            case (TK_PLUS): return {"TK_PLUS"};
            case (TK_MINU): return {"TK_MINU"};
            case (TK_MULT): return {"TK_MULT"};
            case (TK_DIV): return {"TK_DIV"};
            case (TK_MOD): return {"TK_MOD"};
            case (TK_GETINTTK): return {"TK_GETINTTK"};
            case (TK_GETCHARTK): return {"TK_GETCHARTK"};
            case (TK_PRINTFTK): return {"TK_PRINTFTK"};
            case (TK_LSS): return {"TK_LSS"};
            case (TK_LEQ): return {"TK_LEQ"};
            case (TK_GRE): return {"TK_GRE"};
            case (TK_GEQ): return {"TK_GEQ"};
            case (TK_EQL): return {"TK_EQL"};
            case (TK_NEQ): return {"TK_NEQ"};
            case (TK_ASSIGN): return {"TK_ASSIGN"};
            case (TK_SEMICN): return {"TK_SEMICN"};
            case (TK_COMMA): return {"TK_COMMA"};
            case (TK_LPARENT): return {"TK_LPARENT"};
            case (TK_RPARENT): return {"TK_RPARENT"};
            case (TK_LBRACK): return {"TK_LBRACK"};
            case (TK_RBRACK): return {"TK_RBRACK"};
            case (TK_LBRACE): return {"TK_LBRACE"};
            case (TK_RBRACE): return {"TK_RBRACE"};
            case (TK_EOF): return {"(TK_EOF"};
            default: perror("unknown error");
        }
        return "unknown token type";
    }


}
