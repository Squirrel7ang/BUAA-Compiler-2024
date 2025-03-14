//
// Created by tang on 10/16/24.
//

#ifndef PARSER1_HPP
#define PARSER1_HPP

#include <memory>
#include <iostream>
#include "Lexer.hpp"
#include "Ast.hpp"
#include "ErrorReporter.hpp"

namespace tang {

    class Parser1 {
        Lexer& _lexer;
        ErrorReporter& _reporter;
        std::ostream& _correctOutput;
        bool isPrint;

    public:
        explicit Parser1(Lexer& lexer, 
                         ErrorReporter& reporter, 
                         std::ostream& output): 
                        _lexer(lexer), 
                        _reporter(reporter), 
                        _correctOutput(output) {
            isPrint = true;
        }
        u_ptr<CompUnit> parse();


    private:
        u_ptr<BType> _tryBType();
        u_ptr<ConstExp> _tryConstExp();
        u_ptr<StringConst> _tryStringConst();
        u_ptr<ConstInitVal> _tryConstInitVal();
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
        u_ptr<PrimaryExp> _tryPrimaryExp(u_ptr<LVal>& lVal);
        u_ptr<FuncRParams> _tryFuncRParams();
        u_ptr<UnaryOp> _tryUnaryOp();
        u_ptr<FuncCall> _tryFuncCall();
        u_ptr<UnaryExp> _tryUnaryExp();
        u_ptr<UnaryExp> _tryUnaryExp(u_ptr<LVal>& lVal);
        u_ptr<MulExp> _tryMulExp();
        u_ptr<MulExp> _tryMulExp(u_ptr<LVal>&lVal);
        u_ptr<AddExp> _tryAddExp();
        u_ptr<AddExp> _tryAddExp(u_ptr<LVal>& lVal);
        u_ptr<Exp> _tryExp();
        u_ptr<Exp> _tryExp(u_ptr<LVal>& lVal);
        u_ptr<LVal> _tryLVal();
        u_ptr<RelExp> _tryRelExp();
        u_ptr<EqExp> _tryEqExp();
        u_ptr<LAndExp> _tryLAndExp();
        u_ptr<LOrExp> _tryLOrExp();
        u_ptr<Cond> _tryCond();
        u_ptr<Assignment> _tryAssignment();
        u_ptr<ForStmt> _tryForStmt();
        u_ptr<AssignStmt> _tryAssignStmt();
        u_ptr<AssignStmt> _tryAssignStmt(u_ptr<LVal>& lVal);
        u_ptr<IfStmt> _tryIfStmt();
        u_ptr<BreakStmt> _tryBreakStmt();
        u_ptr<ContinueStmt> _tryContinueStmt();
        u_ptr<ReturnStmt> _tryReturnStmt();
        u_ptr<GetintStmt> _tryGetintStmt();
        u_ptr<GetintStmt> _tryGetintStmt(u_ptr<LVal>& lVal);
        u_ptr<GetcharStmt> _tryGetcharStmt();
        u_ptr<GetcharStmt> _tryGetcharStmt(u_ptr<LVal>& lVal);
        u_ptr<PrintfStmt> _tryPrintfStmt();
        u_ptr<Stmt> _tryStmt();
        u_ptr<BlockItem> _tryBlockItem();
        u_ptr<Block> _tryBlock();
        u_ptr<FuncDef> _tryFuncDef();
        u_ptr<Decl> _tryDecl();
        u_ptr<MainFuncDef> _tryMainFuncDef();
        u_ptr<CompUnit> _tryCompUnit();

        [[nodiscard]] Token peekToken(unsigned int) const;
        [[nodiscard]] Token peekToken() const;
        [[nodiscard]] Token getToken() const;
        [[nodiscard]] Token curToken() const;
        void skipToken() const;

        unsigned int _lastLine();

        bool _matchCurToken(TokenType expectType);
        bool _match(Token&& t, TokenType expectType, unsigned int) const;
    };

} // namespace tang

#endif //PARSER1_HPP
