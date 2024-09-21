//
// Created by tang on 9/20/24.
//

#include <string>
#include "tang/token.h"

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

}
