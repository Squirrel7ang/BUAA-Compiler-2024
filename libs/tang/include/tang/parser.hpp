//
// Created by tang on 10/13/24.
//

#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include "lexer.hpp"
#include "ast.hpp"

namespace tang {

    class Parser {
        Lexer& _lexer;

    public:
        explicit Parser(Lexer& lexer): _lexer(lexer) {}
        std::unique_ptr<CompUnit> parse();

    private:
        std::unique_ptr<CompUnit> _parseCompUnit();
        std::unique_ptr<Decl> _parseDecl();
        std::unique_ptr<FuncDef> _parseFuncDef();
    };



} // namespace tang

#endif //PARSER_HPP
