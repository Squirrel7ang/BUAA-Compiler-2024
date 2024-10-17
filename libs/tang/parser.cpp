//
// Created by tang on 10/13/24.
//

#include <memory>

#include "parser.hpp"

#include <cassert>
#include <csignal>

#include "ast.hpp"

namespace tang {
    Token Parser::peekToken(const unsigned int n) const {
        return _lexer.peekToken(n);
    }

    Token Parser::peekToken() const {
        return _lexer.peekToken();
    }

    Token Parser::getToken() const {
        return _lexer.getToken();
    }

    Token Parser::curToken() const {
        return _lexer.curToken();
    }

    void Parser::skipToken(const unsigned int n) const {
        _lexer.skipToken(n);
    }

    void Parser::skipToken() const {
        skipToken(1);
    }


    template <typename T>
    using u_ptr = std::unique_ptr<T>;

    u_ptr<BType> Parser::_tryBType() {
        auto bType = std::make_unique<BType>(peekToken());
        Token&& t = peekToken();

        if (t.getType() == TK_INTTK) {
            bType->isInt = true, bType->isChar = false;
        }
        else if (t.getType() == TK_CHARTK) {
            bType->isChar = true, bType->isInt = false;
        }
        else {
            assert(0);
        }
        skipToken();
        return bType;
    }

    u_ptr<ConstExp> Parser::_tryConstExp() {
        auto constExp = std::make_unique<ConstExp>(peekToken());
        auto addExp = _tryAddExp();
        if (addExp != nullptr) {
            constExp->addExp = std::move(addExp);
        }
        else {
            assert(0);
        }
        return constExp;
    }

    u_ptr<StringConst> Parser::_tryStringConst() {
        auto stringConst = std::make_unique<StringConst>(peekToken());
        Token&& t1 = peekToken();
        if (t1.getType() == TK_STRCON) {
            skipToken();
            stringConst->str = std::make_unique<std::string>(t1.getContent());
        }
        else {
            perror("a stringConst with illegal Token");
        }
        return stringConst;
    }

    u_ptr<ConstInitVal> Parser::_tryConstInitialVal() {
        auto constInitVal = std::make_unique<ConstInitVal>();

        // try ConstExp
        auto constExp = _tryConstExp();
        if (constExp != nullptr) {
            constInitVal->addConstExp(constExp);
            return constInitVal;
        }

        // try StringConst
        auto stringConst = _tryStringConst();
        if (stringConst != nullptr) {
            constInitVal->stringConst = std::move(stringConst);
            return constInitVal;
        }

        // try {ConstExp}
        Token&& t1 = peekToken();
        if (t1.getType() == TK_LBRACE) {
            skipToken();

            auto constExp = _tryConstExp();
            if (constExp != nullptr) {
                constInitVal->addConstExp(constExp);
            }

            bool success = true;
            while (success) {
                t1 = peekToken();
                if (t1.getType() == TK_COMMA) {
                    skipToken();
                }
                else {
                    break;
                }

                constExp = _tryConstExp();
                success = (constExp != nullptr);
                if (success) {
                    constInitVal->addConstExp(constExp);
                }
            }

            t1 = peekToken();
            if (t1.getType() == TK_RBRACE) {
                skipToken();
            }
            else {
                perror("constInitVal expect }");
            }
        }
        return constInitVal;
    }

    u_ptr<ConstDef> Parser::_tryConstDef() {
        auto constDef = std::make_unique<ConstDef>();

        // Ident
        std::unique_ptr<Ident> ident = _tryIdent();
        if (ident != nullptr) {
            constDef->ident = std::move(ident);
        }
        else {
            assert(0);
        }

        // try ConstExp for array type
        Token&& t1 = peekToken();
        if (t1.getType() == TK_LBRACK) {
            skipToken();
            auto constExp = _tryConstExp();
            if (constExp == nullptr) {
                assert(0);
            }
            constDef->constExp = std::move(constExp);
            t1 = peekToken(); // if not TK_RBRACK, skip
            if (t1.getType() == TK_RBRACK) {
                skipToken();
            }
            else {
                perror("constDef expect ]");
            }
        }

        // try '='
        t1 = peekToken();
        if (t1.getType() != TK_ASSIGN) {
            assert(0);
        }
        else {
            skipToken();
        }

        // try ConstInitVal
        auto constInitVal = _tryConstInitialVal();
        if (constInitVal != nullptr) {
            constDef->constInitVal = std::move(constInitVal);
        }
        else {
            assert(0);
        }
        return constDef;
    }

    u_ptr<ConstDecl> Parser::_tryConstDecl() {
        auto constDecl = std::make_unique<ConstDecl>();
        Token&& t1 = _lexer.getToken();
        // BType
        u_ptr<BType> bType = _tryBType();
        if (bType != nullptr) {
            constDecl->bType = std::move(bType);
        }
        else {
            assert(0);
        }

        // ConstDef
        auto constDef = _tryConstDef();
        if (constDef != nullptr) {
            constDecl->constDefs.push_back(std::move(constDef));
        }
        else {
            assert(0);
        }

        bool success = true;
        Token&& t2 = peekToken();
        while (success) {
            t2 = peekToken();
            if (t2.getType() == TK_COMMA) {
                skipToken();
                constDef = _tryConstDef();
                success = (constDef != nullptr);
                if (success) {
                    constDecl->constDefs.push_back(std::move(constDef));
                }
                else {
                    assert(0);
                }
            }
            else {
                break;
            }
        }
        t2 = peekToken();
        if (t2.getType() == TK_SEMICN) {
            skipToken();
        }
        else {
            perror("a constDecl expect ;");
        }
        return constDecl;
    }

    u_ptr<Ident> Parser::_tryIdent() {
        auto ident = std::make_unique<Ident>();
        Token t1 = peekToken();
        if (t1.getType() == TK_IDENFR) {
            skipToken();
            ident->str = std::move(t1.getContent());
        }
        else {
            assert(0);
        }
        return ident;
    }

    u_ptr<InitVal> Parser::_tryInitVal() {
        auto initVal = std::make_unique<InitVal>();

        // tryExp
        auto exp = _tryExp();
        if (exp != nullptr) {
            initVal->addExp(exp);
            return initVal;
        }

        // try StringConst
        auto stringConst = _tryStringConst();
        if (stringConst != nullptr) {
            initVal->stringConst = std::move(stringConst);
        }

        // else try initial value for array with {}
        Token&& t1 = peekToken();
        if (t1.getType() == TK_LBRACE) {
            skipToken();
            exp = _tryExp();
            if (exp == nullptr) {
                // initialize with {} but with no exp inside,
                // thus this is not a InitVal
                return nullptr;
            }
            else {
                initVal->addExp(exp);
            }
            bool success = true;
            while (success) {
                t1 = peekToken();
                if (t1.getType() != TK_COMMA) {
                    break;
                }
                exp = _tryExp();
                success = (exp != nullptr);
                if (success) {
                    initVal->addExp(exp);
                }
            }

            // TK_RBRACE
            t1 = peekToken();
            if (t1.getType() != TK_RBRACE) {
                perror("InitVal {} not close");
            }
            else {
                skipToken();
            }
            return initVal;
        }
        return nullptr;
    }

    u_ptr<VarDef> Parser::_tryVarDef() {
        auto varDef = std::make_unique<VarDef>();

        // try ident
        auto ident = _tryIdent();
        if (ident == nullptr) {
            return nullptr;
        }
        else {
            varDef->ident = std::move(ident);
        }

        // try ConstExp
        Token&& t1 = peekToken();
        if (t1.getType() == TK_LBRACK) {
            skipToken();
            auto constExp = _tryConstExp();
            if (constExp == nullptr) {
                perror("VarDef has illegal ConstExp");
            }
            else {
                varDef->constExp = std::move(constExp);
            }
            t1 = peekToken();
            if (t1.getType() != TK_RBRACK) {
                perror("VarDef ConstExp [] not close");
            }
            else {
                skipToken();
            }
        }

        // try initval
        t1 = peekToken();
        if (t1.getType() == TK_ASSIGN) {
            skipToken();
            auto initVal = _tryInitVal();
            if (initVal == nullptr) {
                perror("VarDef has illegal InitVal");
            }
            else {
                varDef->initVal = std::move(initVal);
            }
        }
        return varDef;
    }

    u_ptr<VarDecl> Parser::_tryVarDecl() {
        auto varDecl = std::make_unique<VarDecl>();

        // try BType
        auto bType = _tryBType();
        if (bType == nullptr) {
            perror("VarDecl has illegal bType");
            return nullptr;
        }
        else {
            varDecl->bType = std::move(bType);
        }

        // try VarDef
        auto varDef = _tryVarDef();
        if (varDef == nullptr) {
            perror("VarDecl has illegal valDef");
            return nullptr;
        }
        else {
            varDecl->varDefs.push_back(std::move(varDef));
        }

        bool success = true;
        while (success) {
            Token&& t1 = peekToken();
            if (t1.getType() != TK_COMMA) {
                break;
            }
            varDef = _tryVarDef();
            success = (varDef != nullptr);
            if (success) {
                varDecl->varDefs.push_back(std::move(varDef));
            }
        }

        return varDecl;

    }

    u_ptr<FuncType> Parser::_tryFuncType() {
        auto funcType = std::make_unique<FuncType>();
        Token&& t1 = peekToken();
        if (t1.getType() == TK_INTTK) {
            funcType->setInt();
        }
        else if (t1.getType() == TK_CHARTK) {
            funcType->setChar();
        }
        else if (t1.getType() == TK_VOIDTK) {
            funcType->setVoid();
        }
        else {
            return nullptr;
        }
        skipToken();
        return funcType;
    }

    u_ptr<FuncFParam> Parser::_tryFuncFParam() {
        auto funcFParam = std::make_unique<FuncFParam>();

        // try BType
        auto bType = _tryBType();
        if (bType == nullptr) {
            perror("FuncFParam has illegal BType");
            return nullptr;
        }
        else {
            funcFParam->bType = std::move(bType);
        }

        // try ident
        auto ident = _tryIdent();
        if (ident == nullptr) {
            perror("FuncFParam has illegal ident");
            return nullptr;
        }
        else {
            funcFParam->ident = std::move(ident);
        }

        // try [];
        Token&& t1 = peekToken();
        if (t1.getType() == TK_LBRACK) {
            skipToken();
            t1 = peekToken();
            if (t1.getType() == TK_RBRACK) {
                skipToken();
            }
            else {
                perror("[] not close");
            }
        }

        return funcFParam;
    }

    u_ptr<FuncFParams> Parser::_tryFuncFParams() {
        auto funcFParams = std::make_unique<FuncFParams>();

        auto funcFParam = _tryFuncFParam();
        if (funcFParam == nullptr) {
            return nullptr;
        }
        else {
            funcFParams->funcFParams.push_back(std::move(funcFParam));
        }

        bool success = true;
        while (success) {
            Token&& t1 = peekToken();
            if (t1.getType() != TK_COMMA) {
                break;
            }
            skipToken();
            funcFParam = _tryFuncFParam();
            success = (funcFParam != nullptr);
            if (success) {
                funcFParams->funcFParams.push_back(std::move(funcFParam));
            }
        }

        return funcFParams;
    }

    u_ptr<IntConst> Parser::_tryIntConst() {
        auto intConst = std::make_unique<IntConst>();
        Token&& t1 = peekToken();
        if (t1.getCol() == TK_INTCON) {
            skipToken();
            intConst->val = std::stoi(t1.getContent());
        }
        else {
            return nullptr;
        }
        return intConst;
    }

    u_ptr<Number> Parser::_tryNumber() {
        auto number = std::make_unique<Number>();

        auto intConst = _tryIntConst();
        if (intConst != nullptr) {
            number->intConst = std::move(intConst);
        }
        else {
            return nullptr;
        }
        return number;
    }

    u_ptr<CharConst> Parser::_tryCharConst() {
        auto charConst = std::make_unique<CharConst>();
        Token&& t1 = peekToken();
        if (t1.getType() == TK_CHARTK) {
            // char token must start with '';
            skipToken();
            charConst->ch = t1.CHRCONToChar();
        }
        else {
            return nullptr;
        }
        return charConst;
    }

    u_ptr<Character> Parser::_tryCharacter() {
        auto character = std::make_unique<Character>();

        auto charConst = _tryCharConst();
        if (charConst != nullptr) {
            character->charConst = std::move(charConst);
        }
        else {
            return nullptr;
        }

        return character;
    }

    u_ptr<PrimaryExp> Parser::_tryPrimaryExp() {
        auto primaryExp = std::make_unique<PrimaryExp>();

        // try LVal;
        auto lVal = _tryLVal();
        if (lVal != nullptr) {
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(std::move(lVal));
            return primaryExp;
        }

        // try Number
        auto number = _tryNumber();
        if (number != nullptr) {
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(std::move(number));
            return primaryExp;
        }

        // try Character
        auto character = _tryCharacter();
        if (character != nullptr) {
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(std::move(character));
            return primaryExp;
        }

        // try (Exp)
        Token&& t1 = peekToken();
        if (t1.getType() == TK_LPARENT) {
            skipToken();

            auto exp = _tryExp();
            if (exp != nullptr) {
                primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(std::move(exp));
            }
            else {
                perror("primaryExp with ( but with none-Exp expression");
                return nullptr;
            }

            t1 = peekToken();
            if (t1.getType() == TK_RPARENT) {
                skipToken();
                return primaryExp;
            }
            else {
                perror("a Primary Exp () not close, skip");
            }
        }
        return nullptr;

    }

    u_ptr<FuncRParams> Parser::_tryFuncRParams() {
        auto funcRParams = std::make_unique<FuncRParams>();

        auto exp = _tryExp();
        if (exp != nullptr) {
            funcRParams->exps.push_back(std::move(exp));
        }
        else {
            return nullptr;
        }

        bool success = true;
        while (success) {
            Token && t1 = peekToken();
            if (t1.getType() != TK_COMMA) {
                break;
            }
            skipToken();
            exp = _tryExp();
            success = (exp != nullptr);
            if (success) {
                funcRParams->exps.push_back(std::move(exp));
            }
        }

        return funcRParams;

    }

    u_ptr<UnaryOp> Parser::_tryUnaryOp() {
        auto unaryOp = std::make_unique<UnaryOp>();
        Token&& t1 = peekToken();
        if (t1.getType() == TK_PLUS) {
            unaryOp->setPlus();
        }
        else if (t1.getType() == TK_MINU) {
            unaryOp->setMinus();
        }
        else if (t1.getType() == TK_NOT) {
            unaryOp->setExc();
        }
        else {
            return nullptr;
        }
        return unaryOp;
    }

    u_ptr<FuncCall> Parser::_tryFuncCall() {
        auto funcCall = std::make_unique<FuncCall>();

        Token&& t1 = peekToken();
        if (t1.getType() != TK_IDENFR) {
            return nullptr;
        }

        skipToken();
        t1 = peekToken();
        if (t1.getType() == TK_LPARENT) {
            skipToken();

            auto funcRParams = _tryFuncRParams();
            funcCall->funcRParams = std::move(funcRParams); // might be nullptr;

            t1 = peekToken();
            if (t1.getType() == TK_RPARENT) {
                skipToken();
            }
            else {
                perror("FuncCall () not close, skip");
            }
        }
        else {
            return nullptr;
        }

        return funcCall;
    }

    u_ptr<UnaryExp> Parser::_tryUnaryExp() {
        auto unaryExp = std::make_unique<UnaryExp>();

        // try unaryOp
        bool success = true;
        while (success) {
            if (peekToken().isUnaryOp()) {
                unaryExp->unaryOps.push_back(_tryUnaryOp());
            }
            else {
                break;
            }
        }

        // try PrimaryExp
        auto primaryExp = _tryPrimaryExp();
        if (primaryExp != nullptr) {
            unaryExp->unaryExp = std::make_unique<UnaryExpVariant>(std::move(primaryExp));
            return unaryExp;
        }

        // try FuncCall;
        auto funcCall = _tryFuncCall();
        if (funcCall != nullptr) {
            unaryExp->unaryExp = std::make_unique<UnaryExpVariant>(std::move(funcCall));
            return unaryExp;
        }

        return nullptr;
    }

    u_ptr<MulExp> Parser::_tryMulExp() {
        auto mulExp = std::make_unique<MulExp>();

        auto unaryExp = _tryUnaryExp();
        if (unaryExp != nullptr) {
            mulExp->unaryExps.push_back(std::move(unaryExp));
        }
        else {
            assert(0);
        }

        bool success = true;
        while (success) {
            Token&& t1 = peekToken();
            if (t1.isMulExpOp()) {
                mulExp->ops.push_back(t1);
            }
            else {
                break;
            }

            unaryExp = _tryUnaryExp();
            success = unaryExp != nullptr;
            if (success) {
                mulExp->unaryExps.push_back(std::move(unaryExp));
            }
            else {
                assert(0);
            }
        }

        return mulExp;
    }

    u_ptr<AddExp> Parser::_tryAddExp() {
        auto addExp = std::make_unique<AddExp>();

        auto mulExp = _tryMulExp();
        if (mulExp != nullptr) {
            addExp->mulExps.push_back(std::move(mulExp));
        }
        else {
            assert(0);
        }

        bool success = true;
        while (success) {
            Token&& t1 = peekToken();
            if (t1.isAddExpOp()) {
                mulExp->ops.push_back(t1);
            }
            else {
                break;
            }

            mulExp = _tryMulExp();
            if (mulExp != nullptr) {
                addExp->mulExps.push_back(std::move(mulExp));
            }
            else {
                assert(0);
            }
        }

        return addExp;
    }

    u_ptr<Exp> Parser::_tryExp() {
        auto exp = std::make_unique<Exp>();

        auto addExp = _tryAddExp();
        if (addExp != nullptr) {
            exp->addExp = std::move(addExp);
        }
        else {
            return nullptr;
        }

        return exp;
    }

    u_ptr<LVal> Parser::_tryLVal() {
        auto lVal = std::make_unique<LVal>();

        auto ident = _tryIdent();
        if (ident != nullptr) {
            lVal->ident = std::move(ident);
        }
        else {
            return nullptr;
        }

        Token&& t1 = peekToken();
        if (t1.getType() == TK_LBRACK) {
            skipToken();

            auto exp = _tryExp();
            if (exp != nullptr) {
                lVal->exp = std::move(exp);
            }
            else {
                perror("lVal expect a exp inside []");
            }

            t1 = peekToken();
            if (t1.getType() == TK_RBRACK) {
                skipToken();
            }
            else {
                perror("lVal [] not close");
            }
        }
        else {
            return lVal;
        }

        return lVal;
    }

    u_ptr<RelExp> Parser::_tryRelExp() {
        auto relExp = std::make_unique<RelExp>();

        auto addExp = _tryAddExp();
        if (addExp != nullptr) {
            relExp->addExps.push_back(std::move(addExp));
        }
        else {
            return nullptr;
        }

        bool success = true;
        while (success) {
            Token&& t1 = peekToken();
            if (t1.isRelExpOp()) {
                relExp->ops.push_back(t1);
            }
            else {
                break;
            }

            addExp = _tryAddExp();
            if (addExp != nullptr) {
                relExp->addExps.push_back(std::move(addExp));
            }
            else {
                perror("relExp expect a addExp");
            }
        }

        return relExp;
    }

    u_ptr<EqExp> Parser::_tryEqExp() {
        auto eqExp = std::make_unique<EqExp>();

        auto relExp = _tryRelExp();
        if (relExp != nullptr) {
            eqExp->relExps.push_back(std::move(relExp));
        }
        else {
            return nullptr;
        }

        bool success = true;
        while (success) {
            Token&& t1 = peekToken();
            if (t1.isEqExpOp()) {
                eqExp->ops.push_back(t1);
            }
            else {
                break;
            }

            relExp = _tryRelExp();
            if (relExp != nullptr) {
                eqExp->relExps.push_back(std::move(relExp));
            }
            else {
                perror("eqExp expect a relExp");
            }
        }

        return eqExp;
    }

    u_ptr<LAndExp> Parser::_tryLAndExp() {
        auto lAndExp = std::make_unique<LAndExp>();

        auto eqExp = _tryEqExp();
        if (eqExp != nullptr) {
            lAndExp->eqExps.push_back(std::move(eqExp));
        }
        else {
            return nullptr;
        }

        bool success = true;
        while (success) {
            Token&& t1 = peekToken();
            if (t1.getType() != TK_AND) {
                break;
            }

            eqExp = _tryEqExp();
            if (eqExp != nullptr) {
                lAndExp->eqExps.push_back(std::move(eqExp));
            }
            else {
                perror("LAndExp expect a eqExp");
            }
        }

        return lAndExp;
    }

    u_ptr<LOrExp> Parser::_tryLOrExp() {
        auto lOrExp = std::make_unique<LOrExp>();

        auto lAndExp = _tryLAndExp();
        if (lAndExp != nullptr) {
            lOrExp->lAndExps.push_back(std::move(lAndExp));
        }
        else {
            return nullptr;
        }

        bool success = true;
        while (success) {
            Token&& t1 = peekToken();
            if (t1.getType() != TK_OR) {
                break;
            }

            lAndExp = _tryLAndExp();
            if (lAndExp != nullptr) {
                lOrExp->lAndExps.push_back(std::move(lAndExp));
            }
            else {
                perror("lOrExp expect a LAndExp");
            }
        }

        return lOrExp;

    }

    u_ptr<Cond> Parser::_tryCond() {
        auto cond = std::make_unique<Cond>();

        auto lOrExp = _tryLOrExp();
        if (lOrExp != nullptr) {
            cond->lOrExp = std::move(lOrExp);
        }
        else {
            return nullptr;
        }
        return cond;
    }

    u_ptr<Assignment> Parser::_tryAssignment() {
        auto assignment = std::make_unique<Assignment>();

        auto lVal = _tryLVal();
        if (lVal != nullptr) {
            assignment->lVal = std::move(lVal);
        }
        else {
            return nullptr;
        }

        if (peekToken().getType() == TK_ASSIGN) {
            skipToken();
        }
        else {
            assert(0);
        }

        auto exp = _tryExp();
        if (exp != nullptr) {
            assignment->exp = std::move(exp);
        }
        else {
            assert(0);
        }

        if (peekToken().getType() == TK_SEMICN) {
            skipToken();
        }
        else {
            perror("Assignment expect Semicon");
        }

        return assignment;
    }

    u_ptr<ForStmt> Parser::_tryForStmt() { // this forStmt is not the one in the document
        auto forStmt = std::make_unique<ForStmt>();

        Token &&t1 = peekToken();
        if (t1.getType() != TK_FORTK) {
            return nullptr;
        }
        skipToken();
        t1 = getToken();
        if (t1.getType() != TK_LPARENT) {
            perror("expect (");
        }

        // try initAssignment;
        auto assignment = _tryAssignment();
        if (assignment != nullptr) {
            forStmt->init = std::move(assignment);
        }
        else {
            perror("illegal for statement Init");
        }

        t1 = peekToken();
        if (t1.getType() == TK_COMMA) {
            skipToken();
        }
        else {
            perror("ForStmt expect comma");
        }

        // try Cond
        auto cond = _tryCond();
        if (cond != nullptr) {
            forStmt->cond = std::move(cond);
        }
        else {
            perror("illegal for statement Cond");
        }

        t1 = peekToken();
        if (t1.getType() == TK_COMMA) {
            skipToken();
        }
        else {
            perror("ForStmt expect comma");
        }

        // try update
        assignment = _tryAssignment();
        if (assignment != nullptr) {
            forStmt->update = std::move(assignment);
        }
        else {
            perror("illegal for statement Init");
        }

        t1 = peekToken();
        if (t1.getType() == TK_RPARENT) {
            skipToken();
        }
        else {
            perror("Forstmt expect )");
        }

        // try Stmt
        auto stmt = _tryStmt();
        if (stmt != nullptr) {
            forStmt->stmt = std::move(stmt);
        }
        else {
            perror("expect stmt");
        }

        return forStmt;
    }

    u_ptr<AssignStmt> Parser::_tryAssignStmt() {
        auto assignStmt = std::make_unique<AssignStmt>();

        auto assignment = _tryAssignment();
        if (assignment != nullptr) {
            assignStmt->assignment = std::move(assignment);
        }
        else {
            return nullptr;
        }

        Token&& t1 = peekToken();
        if (t1.getType() == TK_SEMICN) {
            skipToken();
        }
        else {
            perror("AssignStmt expect ;");
        }

        return assignStmt;
    }

    u_ptr<IfStmt> Parser::_tryIfStmt() {
        auto ifStmt = std::make_unique<IfStmt>();

        if (peekToken().getType() == TK_IFTK) {
            skipToken();
        }
        else {
            return nullptr;
        }

        if (peekToken().getType() == TK_LPARENT) {
            skipToken();
        }
        else {
            assert(0);
        }

        auto cond = _tryCond();
        if (cond != nullptr) {
            ifStmt->cond = std::move(cond);
        }
        else {
            assert(0);
        }

        if (peekToken().getType() == TK_RPARENT) {
            skipToken();
        }
        else {
            assert(0);
        }

        auto stmt = _tryStmt();
        if (stmt != nullptr) {
            ifStmt->ifStmt = std::move(stmt);
        }
        else {
            assert(0);
        }

        if (peekToken().getType() == TK_ELSETK) {
            skipToken();
            stmt = _tryStmt();
            ifStmt->elseStmt = std::move(stmt);
        }
        else {
            ifStmt->elseStmt = nullptr;
        }

        return ifStmt;
    }

    u_ptr<BreakStmt> Parser::_tryBreakStmt() {
        auto breakStmt = std::make_unique<BreakStmt>();

        Token&& t1 = peekToken();
        if (t1.getType() == TK_BREAKTK) {
            skipToken();
        }
        else {
            return nullptr;
        }

        // try semicolon
        t1 = peekToken();
        if (t1.getType() == TK_SEMICN) {
            skipToken();
        }
        else {
            perror("semicon");
        }

        return breakStmt;
    }

    u_ptr<ContinueStmt> Parser::_tryContinueStmt() {
        auto continueStmt = std::make_unique<ContinueStmt>();

        Token&& t1 = peekToken();
        if (t1.getType() == TK_CONTINUETK) {
            skipToken();
        }
        else {
            return nullptr;
        }

        // try semicolon
        t1 = peekToken();
        if (t1.getType() == TK_SEMICN) {
            skipToken();
        }
        else {
            perror("semicon");
        }

        return continueStmt;

    }

    u_ptr<ReturnStmt> Parser::_tryReturnStmt() {
        auto returnStmt = std::make_unique<ReturnStmt>();

        Token&& t1 = peekToken();
        if (t1.getType() == TK_RETURNTK) {
            skipToken();
        }
        else {
            return nullptr;
        }

        // try exp
        auto exp = _tryExp();
        if (exp != nullptr) {
            returnStmt->exp = std::move(exp);
        }

        // try semicon
        t1 = peekToken();
        if (t1.getType() == TK_SEMICN) {
            skipToken();
        }
        else {
            perror("semicon");
        }

        return returnStmt;
    }

    u_ptr<GetintStmt> Parser::_tryGetintStmt() {
        auto getintStmt = std::make_unique<GetintStmt>();

        auto lVal = _tryLVal();
        if (lVal != nullptr) {
            getintStmt->lVal = std::move(lVal);
        }
        else {
            return nullptr;
        }

        // try '='
        Token&& t1 = peekToken();
        if (t1.getType() == TK_ASSIGN) {
           skipToken();
        }
        else {
            return nullptr;
        }

        // try getint();
        t1 = peekToken();
        if (t1.getType() == TK_GETINTTK) {
            skipToken();
        }
        else {
            return nullptr;
        }

        // try the rest, "();"
        if (peekToken().getType() == TK_LPARENT) {
            skipToken();
        }
        if (peekToken().getType() == TK_RPARENT) {
            skipToken();
        }
        if (peekToken().getType() == TK_SEMICN) {
            skipToken();
        }

        return getintStmt;
    }

    u_ptr<GetcharStmt> Parser::_tryGetcharStmt() {
        auto getcharStmt = std::make_unique<GetcharStmt>();

        auto lVal = _tryLVal();
        if (lVal != nullptr) {
            getcharStmt->lVal = std::move(lVal);
        }
        else {
            return nullptr;
        }

        // try '='
        Token&& t1 = peekToken();
        if (t1.getType() == TK_ASSIGN) {
           skipToken();
        }
        else {
            return nullptr;
        }

        // try getchar
        t1 = peekToken();
        if (t1.getType() == TK_GETCHARTK) {
            skipToken();
        }
        else {
            return nullptr;
        }

        // try the rest, "();"
        if (peekToken().getType() == TK_LPARENT) {
            skipToken();
        }
        if (peekToken().getType() == TK_RPARENT) {
            skipToken();
        }
        if (peekToken().getType() == TK_SEMICN) {
            skipToken();
        }

        return getcharStmt;
    }

    u_ptr<PrintfStmt> Parser::_tryPrintfStmt() {
        auto printfStmt = std::make_unique<PrintfStmt>();

        Token&& t1 = peekToken();
        if (t1.getType() == TK_PRINTFTK) {
            skipToken();
        }
        else {
            return nullptr;
        }

        // try StringConst
        auto stringConst = _tryStringConst();
        if (stringConst != nullptr) {
            printfStmt->stringConst = std::move(stringConst);
        }
        else {
            perror("PrintfStmt expect stringConst");
        }

        // try many Exp;
        bool success = true;
        while (success) {
            t1 = peekToken();
            if (t1.getType() == TK_COMMA) {
                skipToken();
            }
            else {
                break;
            }

            // tryExp
            auto exp = _tryExp();
            success = (exp != nullptr);
            if (success) {
                printfStmt->exps.push_back(std::move(exp));
            }
        }

        return printfStmt;
    }

    u_ptr<Stmt> Parser::_tryStmt() {
        auto stmt = std::make_unique<Stmt>();

        // try assignStmt
        auto assignStmt = _tryAssignStmt();
        if (assignStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(assignStmt));
            return stmt;
        }

        // try Exp
        auto exp = _tryExp();
        if (exp != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(exp));
            return stmt;
        }
        else {
            Token&& t1 = peekToken();
            if (t1.getType() == TK_SEMICN) {
                skipToken();
                stmt->stmt = std::make_unique<StmtVariant>(std::move(exp));
                return stmt;
            }
        }

        // try IfStmt
        auto ifStmt = _tryIfStmt();
        if (ifStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(ifStmt));
            return stmt;
        }

        // try ForStmt
        auto forStmt = _tryForStmt();
        if (forStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(forStmt));
            return stmt;
        }

        // try break
        auto breakStmt = _tryBreakStmt();
        if (breakStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(breakStmt));
            return stmt;
        }

        // try continueStmt;
        auto continueStmt = _tryContinueStmt();
        if (continueStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(continueStmt));
            return stmt;
        }

        // try returnStmt;
        auto returnStmt = _tryReturnStmt();
        if (returnStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(returnStmt));
            return stmt;
        }

        // try getintStmt;
        auto getintStmt = _tryGetintStmt();
        if (getintStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(getintStmt));
            return stmt;
        }

        // try getcharStmt;
        auto getcharStmt = _tryGetcharStmt();
        if (getcharStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(getcharStmt));
            return stmt;
        }

        // try printfStmt;
        auto printfStmt = _tryPrintfStmt();
        if (continueStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(printfStmt));
            return stmt;
        }

        return nullptr;
    }

    u_ptr<BlockItem> Parser::_tryBlockItem() {
        auto blockItem = std::make_unique<BlockItem>();

        // try decl
        auto decl = _tryDecl();
        if (decl != nullptr) {
            blockItem->blockItem = std::make_unique<BlockItemVariant>(std::move(decl));
            return blockItem;
        }

        // try Stmt
        auto stmt = _tryStmt();
        if (stmt != nullptr) {
            blockItem->blockItem = std::make_unique<BlockItemVariant>(std::move(stmt));
            return blockItem;
        }

        return nullptr;
    }

    u_ptr<Block> Parser::_tryBlock() {
        auto block = std::make_unique<Block>();

        Token&& t1 = peekToken();
        if (t1.getType() == TK_LBRACE) {
            skipToken();

            auto blockItem = _tryBlockItem();
            if (blockItem != nullptr) {
                block->blockItem = std::move(blockItem);
            }
            else {
                perror("Block expect blockItem");
            }

            t1 = peekToken();
            if (t1.getType() == TK_RBRACE) {
                skipToken();
            }
            else {
                perror("Block {} not closed");
            }
        }
        else {
            return nullptr;
        }

        return block;
    }

    u_ptr<FuncDef> Parser::_tryFuncDef() {
        auto funcDef = std::make_unique<FuncDef>();

        auto funcType = _tryFuncType();
        if (funcType != nullptr) {
            funcDef->funcType = std::move(funcType);
        }
        else {
            return nullptr;
        }

        auto ident = _tryIdent();
        if (ident != nullptr) {
            funcDef->ident = std::move(ident);
        }
        else {
            return nullptr;
        }

        Token&& t1 = peekToken();
        if (t1.getType() == TK_LPARENT) {
            skipToken();
            auto funcFParams = _tryFuncFParams();
            if (funcFParams != nullptr) {
                funcDef->funcFParams = std::move(funcFParams);
            }
            t1 = peekToken();
            if (t1.getType() == TK_RPARENT) {
                skipToken();
            }
            else {
                perror("FuncDef expect )");
            }
            // try Block
            auto block = _tryBlock();
            if (block != nullptr) {
                funcDef->block = std::move(block);
            }
            else {
                perror("funcDef expect Block");
            }
        }
        else {
            return nullptr;
        }

        return funcDef;
    }

    u_ptr<Decl> Parser::_tryDecl() {
        u_ptr<Decl> decl;
        Token&& t1 = _lexer.peekToken();
        if (t1.isConstTK()) {
            auto constDecl = _tryConstDecl();
            if (constDecl != nullptr) {
                decl->decl = std::make_unique<DeclVariant>(std::move(constDecl));
                return decl;
            }
            else {
                assert(0);
            }
        }
        else {
            // this is a varDecl
            auto varDecl = _tryVarDecl();
            if (varDecl != nullptr) {
                decl->decl = std::make_unique<DeclVariant>(std::move(varDecl));
                return decl;
            }
            else {
                assert(0);
            }
        }
        perror("a decl is neither constDecl nor varDecl");
        return nullptr;
    }

    u_ptr<MainFuncDef> Parser::_tryMainFuncDef() {
        auto mainFuncDef = std::make_unique<MainFuncDef>();

        Token&& t1 = peekToken();
        if (t1.getType() == TK_INTTK) {
            skipToken();
        }
        else {
            perror("MainFuncDef expect intToken");
            return nullptr;
        }

        t1 = peekToken();
        if (t1.getType() == TK_MAINTK) {
            skipToken();
        }
        else {
            perror("MainFuncDef expect intToken");
            return nullptr;
        }

        if (peekToken().getType() == TK_LPARENT) {
            skipToken();
        }
        else {
            assert(0);
        }

        if (peekToken().getType() == TK_RPARENT) {
            skipToken();
        }
        else {
            assert(0);
        }

        auto block = _tryBlock();
        if (block != nullptr) {
            mainFuncDef->block = std::move(block);
        }
        else {
            assert(0);
        }

        return mainFuncDef;

    }

    u_ptr<CompUnit> Parser::_tryCompUnit() {
        auto compUnit = std::make_unique<CompUnit>();
        Token&& t = peekToken();

        compUnit->setLin(t.getLin());
        compUnit->setCol(t.getCol());

        while (1) {
            Token && t1 = peekToken(0);
            Token && t2 = peekToken(1);
            Token && t3 = peekToken(2);

            if (t1.isConstTK() ||
                t1.isBType() && t2.getType() == TK_IDENFR && t3.getType() != TK_LPARENT) {
                compUnit->decls.push_back(_tryDecl());
            }
            else {
                break;
            }
        }

        while (1) {
            Token && t1 = peekToken(0);
            Token && t2 = peekToken(1);
            Token && t3 = peekToken(2);

            if (t1.isFuncType() && t2.getType() == TK_IDENFR && t3.getType() == TK_LPARENT) {
                compUnit->funcDefs.push_back(_tryFuncDef());
            }
            else {
                break;
            }
        }

        Token && t1 = peekToken(0);
        Token && t2 = peekToken(1);

        if (t1.getType() == TK_INTTK && t2.getType() == TK_MAINTK) {
            compUnit->mainFuncDef = _tryMainFuncDef();
        }

        return compUnit;

    }

    u_ptr<CompUnit> Parser::parse() {
        auto compUnit = _tryCompUnit();
        if (compUnit != nullptr) {
            return compUnit;
        }
        else {
            perror("CompUnit failed");
            exit(1);
        }
    }

} // namespace tang
