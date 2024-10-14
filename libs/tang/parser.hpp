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
        u_ptr<BType> _tryBType();
        u_ptr<ConstExp> _tryConstExp();
        u_ptr<StringConst> _tryStringConst();
        u_ptr<ConstInitVal> _tryConstInitialVal();
        u_ptr<ConstDef> _tryConstDef();
        u_ptr<ConstDecl> _tryConstDecl();
        u_ptr<Ident> _tryIdent();
        u_ptr<InitVal> _tryInitVal();
        u_ptr<VarDef> _tryVarDef();
        u_ptr<VarDecl> _tryVarDecl();
        u_ptr<FuncType> _tryFuncType();
        u_ptr<FuncFParam> _tryFuncFParam();
        u_ptr<FuncFParams> _tryFuncFParams();
        u_ptr<IntConst> _tryIntConst();
        u_ptr<Number> _tryNumber();
        u_ptr<CharConst> _tryCharConst();
        u_ptr<Character> _tryCharacter();
        u_ptr<PrimaryExp> _tryPrimaryExp();
        u_ptr<FuncRParams> _tryFuncRParams();
        u_ptr<UnaryOp> _tryUnaryOp();
        u_ptr<UnaryExp> _tryUnaryExp();
        u_ptr<MulExp> _tryMulExp();
        u_ptr<AddExp> _tryAddExp();
        u_ptr<Exp> _tryExp();
        u_ptr<LVal> _tryLVal();
        u_ptr<RelExp> _tryRelExp();
        u_ptr<EqExp> _tryEqExp();
        u_ptr<LAndExp> _tryLAndExp();
        u_ptr<LOrExp> _tryLOrExp();
        u_ptr<Cond> _tryCond();
        u_ptr<ForStmt> _tryForStmt();
        u_ptr<Stmt> _tryStmt();
        u_ptr<BlockItem> _tryBlockItem();
        u_ptr<Block> _tryBlock();
        u_ptr<FuncDef> _tryFuncDef();
        u_ptr<Decl> _tryDecl();

        u_ptr<MainFuncDef> _tryMainFuncDef();

        std::unique_ptr<CompUnit> _tryCompUnit();
        std::unique_ptr<Decl> _parseDecl();
        std::unique_ptr<FuncDef> _parseFuncDef();

        [[nodiscard]] Token peekToken(unsigned int) const;
        [[nodiscard]] Token peekToken() const;
        [[nodiscard]] Token getToken() const;
        [[nodiscard]] Token curToken() const;
        void skipToken(unsigned int) const;
        void skipToken() const;

    };



} // namespace tang

#endif //PARSER_HPP
