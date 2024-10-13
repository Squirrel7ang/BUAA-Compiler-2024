//
// Created by tang on 10/13/24.
//

#include <memory>

#include "parser.hpp"
#include "ast.hpp"

namespace tang {
    std::unique_ptr<CompUnit> Parser::parse() {
        std::unique_ptr<CompUnit> ptr;
        bool success = _tryCompUnit(ptr);
        if (success) {
            return ptr;
        }
        else {
            perror("CompUnit failed");
            exit(1);
        }

    }

    std::unique_ptr<CompUnit> Parser::_tryCompUnit(std::unique_ptr<CompUnit>& compUnit) {
        Token&& t = _lexer.peekToken();
        bool success = true;

        compUnit = std::make_unique<CompUnit>();
        compUnit->setLin(t.getLin());
        compUnit->setCol(t.getCol());

        // parse decl
        while (success) {
            std::unique_ptr<Decl> decl;
            success = _tryDecl(decl);
            if (success) {
                compUnit.addDecl(std::move(decl));
            }
        }

    }




} // namespace tang
