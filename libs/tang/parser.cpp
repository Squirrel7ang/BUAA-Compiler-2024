//
// Created by tang on 10/13/24.
//

#include <memory>

#include "parser.hpp"

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
        auto bType = std::make_unique<BType>();
        Token&& t = peekToken();

        if (t.getType() == TK_INTTK) {
            bType->isInt = true, bType->isChar = false;
        }
        else if (t.getType() == TK_CHARTK) {
            bType->isChar = true, bType->isInt = false;
        }
        else {
            perror("a BType is neither int nor char");
            return nullptr;
        }
        skipToken();
        return bType;
    }

    u_ptr<ConstExp> Parser::_tryConstExp() {
        auto constExp = std::make_unique<ConstExp>();
        auto addExp = _tryAddExp();
        if (addExp != nullptr) {
            constExp->addExp = std::move(addExp);
        }
        else {
            perror("a constExp with illegal AddExp");
        }
        return constExp;
    }

    u_ptr<StringConst> Parser::_tryStringConst() {
        auto stringConst = std::make_unique<StringConst>();
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
        }
        if (t1.getType() == TK_RBRACE) {
            // TODO
        }
         // TODO
        return nullptr;
    }

    u_ptr<ConstDef> Parser::_tryConstDef() {
        auto constDef = std::make_unique<ConstDef>();

        // Ident
        std::unique_ptr<Ident> ident = _tryIdent();
        if (ident != nullptr) {
            constDef->ident = std::move(ident);
        }

        // try ConstExp for array type
        Token&& t1 = peekToken();
        if (t1.getType() == TK_LBRACK) {
            skipToken();
            auto constExp = _tryConstExp();
            if (constExp == nullptr) {
                perror("constDef with illegal ConstExp");
            }
            constDef->constExp = std::move(constExp);
            t1 = peekToken(); // if not TK_RBRACK, skip
            if (t1.getType() != TK_RBRACK) {
                perror("constDef for array [] not close, skip");
            }
            else {
                skipToken();
            }
        }

        // try '='
        t1 = peekToken();
        if (t1.getType() != TK_ASSIGN) {
            perror("a constDef without '=' to indicate initial value");
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
            perror("a constDef without initial value");
        }
        return constDef;
    }

    u_ptr<ConstDecl> Parser::_tryConstDecl() {
        auto constDecl = std::make_unique<ConstDecl>();
        Token&& t1 = _lexer.getToken();
        if (t1.getType() != TK_CONSTTK) {
            perror("a constdecl start without const");
        }

        // BType
        u_ptr<BType> bType = _tryBType();
        if (bType != nullptr) {
            constDecl->bType = std::make_unique<BType>(*bType);
        }
        else {
            perror("a constDecl with illegal BType");
            return nullptr;
        }

        // ConstDef
        auto constDef = _tryConstDef();
        if (constDef != nullptr) {
            constDecl->constDefs.push_back(std::move(constDef));
        }
        else {
            perror("a constDecl with illegal first ConstDef");
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
            // TODO
            perror("a constDecl without semicolon");
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
            perror("Ident with illegal token");
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
                varDecl->varDefs.push_back(varDef);
            }
        }

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
            funcFParams->funcFParams.push_back(funcFParam);
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
                funcFParams->funcFParams.push_back(funcFParam);
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
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(std::move(*lVal));
            return primaryExp;
        }

        // try Number
        auto number = _tryNumber();
        if (number != nullptr) {
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(std::move(*number));
            return primaryExp;
        }

        // try Character
        auto character = _tryCharacter();
        if (character != nullptr) {
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(std::move(*character));
            return primaryExp;
        }

        // try (Exp)
        Token&& t1 = peekToken();
        if (t1.getType() == TK_LPARENT) {
            skipToken();

            auto exp = _tryExp();
            if (exp != nullptr) {
                primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(std::move(*exp));
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
            funcRParams->exps.push_back(exp);
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
                funcRParams->exps.push_back(exp);
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
            auto unaryOp = _tryUnaryOp();
            success = (unaryOp != nullptr);
            if (success) {
                unaryExp->unaryOps.push_back(unaryOp);
            }
        }

        // try PrimaryExp
        auto primaryExp = _tryPrimaryExp();
        if (primaryExp != nullptr) {
            unaryExp->unaryExp = std::make_unique<UnaryExpVariant>(std::move(*primaryExp));
            return unaryExp;
        }

        // try FuncCall;
        auto funcCall = _tryFuncCall();
        if (funcCall != nullptr) {
            unaryExp->unaryExp = std::make_unique<UnaryExpVariant>(std::move(*funcCall));
            return unaryExp;
        }

        return nullptr;
    }

    u_ptr<MulExp> Parser::_tryMulExp() {
        auto mulExp = std::make_unique<MulExp>();

        auto unaryExp = _tryUnaryExp();
        if (unaryExp != nullptr) {
            mulExp->unaryExps.push_back(unaryExp);
        }
        else {
            return nullptr;
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
            if (unaryExp != nullptr) {
                mulExp->unaryExps.push_back(unaryExp);
            }
            else {
                perror("mulExp expect a unaryExp");
            }
        }

        return mulExp;
    }

    u_ptr<AddExp> Parser::_tryAddExp() {
        auto addExp = std::make_unique<AddExp>();

        auto mulExp = _tryMulExp();
        if (mulExp != nullptr) {
            addExp->mulExps.push_back(mulExp);
        }
        else {
            return nullptr;
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
                addExp->mulExps.push_back(mulExp);
            }
            else {
                perror("addExp expect a mulExp");
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
            relExp->addExps.push_back(addExp);
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
                relExp->addExps.push_back(addExp);
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
            eqExp->relExps.push_back(relExp);
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
                eqExp->relExps.push_back(relExp);
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
            lAndExp->eqExps.push_back(eqExp);
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
                lAndExp->eqExps.push_back(eqExp);
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
            lOrExp->lAndExps.push_back(lAndExp);
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
                lOrExp->lAndExps.push_back(lAndExp);
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

    u_ptr<Assignment> _tryAssignment() {

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

    u_ptr<Stmt> Parser::_tryStmt() {
        auto stmt = std::make_unique<Stmt>();

        // try assignStmt
        auto assignStmt = _tryAssignStmt();
        if (assignStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(*assignStmt));
            return stmt;
        }

        // try Exp
        auto exp = _tryExp();
        if (exp != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(*exp));
            return stmt;
        }
        else {
            Token&& t1 = peekToken();
            if (t1.getType() == TK_SEMICN) {
                skipToken();
                stmt->stmt = std::make_unique<StmtVariant>(std::move(*exp));
                return stmt;
            }
        }

        // try IfStmt
        auto ifStmt = _tryifStmt();
        if (ifStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(*ifStmt));
            return stmt;
        }

        // try ForStmt
        auto forStmt = _tryForStmt();
        if (forStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(*forStmt));
            return stmt;
        }

        // try break
        auto breakStmt = _tryBreakStmt();
        if (breakStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(*breakStmt));
            return stmt;
        }

        // try continueStmt;
        auto continueStmt = _tryContinueStmt();
        if (continueStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(*continueStmt));
            return stmt;
        }

        // try returnStmt;
        auto returnStmt = _tryReturnStmt();
        if (returnStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(*returnStmt));
            return stmt;
        }

        // try getintStmt;
        auto getintStmt = _tryGetintStmt();
        if (getintStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(*getintStmt));
            return stmt;
        }

        // try getcharStmt;
        auto getcharStmt = _tryGetcharStmt();
        if (getcharStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(*getcharStmt));
            return stmt;
        }

        // try printfStmt;
        auto printfStmt = _tryPrintfStmt();
        if (continueStmt != nullptr) {
            stmt->stmt = std::make_unique<StmtVariant>(std::move(*printfStmt));
            return stmt;
        }

        return nullptr;
    }

    u_ptr<BlockItem> Parser::_tryBlockItem() {
        auto blockItem = std::make_unique<BlockItem>();

        // try decl
        auto decl = _tryDecl();
        if (decl != nullptr) {
            blockItem->blockItem = std::make_unique<BlockItemVariant>(std::move(*decl));
            return blockItem;
        }

        // try Stmt
        auto stmt = _tryStmt();
        if (stmt != nullptr) {
            blockItem->blockItem = std::make_unique<BlockItemVariant>(std::move(*stmt));
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
            // TODO
        }

        Token // TODO

    }

    u_ptr<Decl> Parser::_tryDecl() {
        u_ptr<Decl> decl;
        Token&& t1 = _lexer.peekToken(1);
        Token&& t2 = _lexer.peekToken(2);
        Token&& t3 = _lexer.peekToken(3);
        bool constFlag = false;
        if (t1.isConstTK()) {
            auto constDecl = _tryConstDecl();
            if (constDecl != nullptr) {
                decl->decl = std::make_unique<DeclVariant>(std::move(*constDecl));
                return decl;
            }
        }
        else {
            // this is a varDecl
            auto varDecl = _tryVarDecl();
            if (varDecl != nullptr) {
                decl->decl = std::make_unique<DeclVariant>(std::move(*varDecl));
                return decl;
            }
        }
        perror("a decl is neither constDecl nor varDecl");
    }

    u_ptr<MainFuncDef> Parser::_tryMainFuncDef() {

    }

    u_ptr<CompUnit> Parser::_tryCompUnit() {
        auto compUnit = std::make_unique<CompUnit>();
        Token&& t = _lexer.peekToken();

        compUnit->setLin(t.getLin());
        compUnit->setCol(t.getCol());

        // parse decl
        bool success = true;
        while (success) {
            u_ptr<Decl> decl = std::move(_tryDecl());
            success = (decl != nullptr);
            if (success)
                compUnit->decls.push_back(decl);
        }

        success = true;
        while (success) {
            u_ptr<FuncDef> funcDef = std::move(_tryFuncDef());
            success = (funcDef != nullptr);
            if (success)
                compUnit->funcDefs.push_back(funcDef);
        }

        u_ptr<MainFuncDef> mainFuncDef = std::move(_tryMainFuncDef());
        success = (mainFuncDef != nullptr);
        if (success)
            compUnit->mainFuncDef = std::move(mainFuncDef);

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
