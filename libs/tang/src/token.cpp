//
// Created by tang on 9/20/24.
//

#include <string>
#include "tang/token.hpp"

namespace tang {
    Token::Token(const std::string_view &str,
        const std::string_view& filename,
        const unsigned int lin,
        const unsigned int col,
        const TokenType tokenType)
        : content(str), _filename(filename), _lin(lin), _col(col), type(tokenType) {
    }

    inline unsigned int Token::getCol() const {
        return this->_col;
    }

    inline unsigned int Token::getLin() const {
        return this->_lin;
    }

    inline std::string_view Token::getContent() const {
        return this->content;
    }

    inline std::string_view Token::getFilename() const {
        return this->_filename;
    }

    inline TokenType Token::getType() const {
        return this->type;
    }
}
