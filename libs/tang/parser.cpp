//
// Created by tang on 10/13/24.
//

#include <memory>

#include "parser.hpp"
#include "ast.hpp"

namespace tang {
    template <typename T>
    using u_ptr = std::unique_ptr<T>;

    u_ptr<BType> Parser::_tryBType() {
        u_ptr<BType> bType = std::make_unique<BType>();
        Token&& t = _lexer.peekToken();
        if (t.getType() == TK_INTTK) {
            bType.addInt();
        }
        else if (t.getType() == TK_CHARTK) {
            bType.addChar();
        }
        else
            return nullptr;
        return bType;
    }

    u_ptr<ConstExp> Parser::_tryConstExp() {

    }

    u_ptr<StringConst> Parser::_tryStringConst() {

    }

    u_ptr<ConstInitVal> Parser::_tryConstInitialVal() {

    }

    u_ptr<ConstDef> Parser::_tryConstDef() {

    }

    u_ptr<ConstDecl> Parser::_tryConstDecl() {

    }

    u_ptr<Ident> Parser::_tryIdent() {

    }

    u_ptr<InitVal> Parser::_tryInitVal() {

    }

    u_ptr<VarDef> Parser::_parseVarDef() {

    }

    u_ptr<VarDecl> Parser::_tryVarDecl() {

    }

    u_ptr<FuncType> Parser::_tryFuncType() {

    }

    u_ptr<FuncFParam> Parser::_tryFuncFParam() {

    }

    u_ptr<FuncFParams> Parser::_tryFuncFParams() {

    }

    u_ptr<IntConst> Parser::_tryIntConst() {

    }

    u_ptr<Number> Parser::_tryNumber() {

    }

    u_ptr<CharConst> Parser::_tryCharConst() {

    }

    u_ptr<Character> Parser::_tryCharacter() {

    }

    u_ptr<PrimaryExp> Parser::_tryPrimaryExp() {

    }

    u_ptr<FuncRParams> Parser::_tryFuncRParams() {

    }

    u_ptr<UnaryOp> Parser::_tryUnaryOp() {

    }

    u_ptr<UnaryExp> Parser::_tryUnaryExp() {

    }

    u_ptr<MulExp> Parser::_tryMulExp() {

    }

    u_ptr<AddExp> Parser::_tryAddExp() {

    }

    u_ptr<Exp> Parser::_tryExp() {

    }

    u_ptr<LVal> Parser::_tryLVal() {

    }

    u_ptr<RelExp> Parser::_tryRelExp() {

    }

    u_ptr<EqExp> Parser::_tryEqExp() {

    }

    u_ptr<LAndExp> Parser::_tryLAndExp() {

    }

    u_ptr<LOrExp> Parser::_tryLOrExp() {

    }

    u_ptr<Cond> Parser::_tryCond() {

    }

    u_ptr<ForStmt> Parser::_tryForStmt() {

    }

    u_ptr<Stmt> Parser::_tryStmt() {

    }

    u_ptr<BlockItem> Parser::_tryBlockItem() {

    }

    u_ptr<Block> Parser::_tryBlock() {

    }

    u_ptr<FuncDef> Parser::_tryFuncDef() {

    }

    u_ptr<Decl> Parser::_tryDecl() {
        Token&& t1 = _lexer.peekToken(1);
        Token&& t2 = _lexer.peekToken(2);
        Token&& t3 = _lexer.peekToken(3);
        bool constFlag = false;
        if (t1.isConstTK()) {
            auto&& constDecl = _tryConstDecl();
            auto decl = std::make_unique<Decl>();
            decl->addConstDecl(constDecl);
            return decl;
        }
        else if (t1.isFuncType() && t2.getType() == TK_IDENFR && t3.getType() == TK_LPARENT) {
            // this is a function declaration
            return nullptr;
        }
        else {
            // this is a vardeclaration

        }
    }

    u_ptr<CompUnit> Parser::_tryCompUnit() {
        Token&& t = _lexer.peekToken();

        auto compUnit = std::make_unique<CompUnit>();
        compUnit->setLin(t.getLin());
        compUnit->setCol(t.getCol());

        // parse decl
        bool success = true;
        while (success) {
            u_ptr<Decl> decl;
            success = _tryDecl(decl);
            if (success) {
                compUnit->addDecl(decl);
            }
        }

        success = true;
        while (success) {
            u_ptr<FuncDef> funcDef;
            success = _tryFuncDef(funcDef);
            if (success) {
                compUnit->addFuncDef(funcDef);
            }
        }

        return compUnit;
    }

    u_ptr<CompUnit> Parser::parse() {
        u_ptr<CompUnit> ptr;
        bool success = _tryCompUnit(ptr);
        if (success) {
            return ptr;
        }
        else {
            perror("CompUnit failed");
            exit(1);
        }

    }



} // namespace tang
