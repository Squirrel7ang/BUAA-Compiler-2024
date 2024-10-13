//
// Created by tang on 10/13/24.
//

#include <memory>

#include "tang/parser.hpp"

namespace tang {
    std::unique_ptr<CompUnit> Parser::parse() {
        return _parseCompUnit();

    }

    std::unique_ptr<CompUnit> Parser::_parseCompUnit() {
        auto compUnit = std::make_unique<CompUnit>();
        Token t = _lexer.nextToken();
        compUnit->setLin(t.getLin());
        compUnit->setCol(t.getCol());

        // parse decl
        while (1 /* TODO */ ) {
            // if this is a const declaration
            if (t.getType() == TK_CONSTTK) {


            }

            t = _lexer.nextToken();

            if (t.isFuncType()) {

            }
            else {
                // a CompUnit must start with an 'int' 'char' 'void' or 'const'
                perror("CompUnitError");
            }

        }

    }




} // namespace tang
