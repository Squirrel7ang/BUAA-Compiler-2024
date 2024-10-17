//
// Created by tang on 10/16/24.
//

#include "parser1.hpp"

#include <cassert>

#include "ast.hpp"

namespace tang {
    Token Parser1::peekToken(const unsigned int n) const {
        return _lexer.peekToken(n);
    }

    Token Parser1::peekToken() const {
        return _lexer.peekToken();
    }

    Token Parser1::getToken() const {
        peekToken().print(_correctOutput);
        return _lexer.getToken();
    }

    Token Parser1::curToken() const {
        return _lexer.curToken();
    }

    void Parser1::skipToken() const {
        peekToken().print(_correctOutput);
        _lexer.skipToken(1);
    }

    u_ptr<BType> Parser1::_tryBType()  {
        auto bType = std::make_unique<BType>(peekToken());
        Token&& t1 = getToken();
        if (t1.getType() == TK_INTTK) {
            bType->setInt();
        }
        else if (t1.getType() == TK_CHARTK) {
            bType->setChar();
        }
        else {
            assert(0);
        }

        BType::print(_correctOutput);
        return bType;
    }

    u_ptr<ConstExp> Parser1::_tryConstExp()  {
        auto constExp = std::make_unique<ConstExp>(peekToken());
        constExp->addExp = _tryAddExp();
        ConstExp::print(_correctOutput);
        return constExp;
    }

    u_ptr<StringConst> Parser1::_tryStringConst()  {
        auto stringConst = std::make_unique<StringConst>(peekToken());
        assert(peekToken().getType() == TK_STRCON);

        Token&& t1 = getToken();

        std::string&& content = t1.STRCONToString();
        stringConst->str = std::make_unique<std::string>(content);

        StringConst::print(_correctOutput);
        return stringConst;
    }

    u_ptr<ConstInitVal> Parser1::_tryConstInitVal()  {
        auto constInitVal = std::make_unique<ConstInitVal>(peekToken());

        Token && t1 = peekToken();
        // try stringConst
        if (t1.getType() == TK_STRCON) {
            constInitVal->stringConst = _tryStringConst();
        }
        else if (t1.getType() == TK_LBRACE) {
            skipToken();
            auto constExp = _tryConstExp();
            constInitVal->addConstExp(constExp);

            while (t1.isComma()) {
                skipToken();
                constExp = _tryConstExp();
                constInitVal->addConstExp(constExp);
            }

            _matchCurToken(TK_RBRACE);
        }
        else {
            // ConstExp;
            auto constExp = _tryConstExp();
            constInitVal->addConstExp(constExp);
        }

        ConstInitVal::print(_correctOutput);
        return constInitVal;
    }

    u_ptr<ConstDef> Parser1::_tryConstDef()  {
        auto constDef = std::make_unique<ConstDef>(peekToken());

        assert(peekToken().getType() == TK_IDENFR);

        constDef->ident = _tryIdent();
        if (peekToken().getType() == TK_LBRACK) {
            skipToken();
            constDef->constExp = _tryConstExp();

            _matchCurToken(TK_RBRACE);
        }
        _matchCurToken(TK_ASSIGN);
        constDef->constInitVal = _tryConstInitVal();

        ConstDef::print(_correctOutput);
        return constDef;
    }

    u_ptr<ConstDecl> Parser1::_tryConstDecl()  {
        auto constDecl = std::make_unique<ConstDecl>(peekToken());
        assert(peekToken().getType() == TK_CONSTTK);

        _matchCurToken(TK_CONSTTK);
        constDecl->bType = _tryBType();
        constDecl->constDefs.push_back(_tryConstDef());
        while (peekToken().isComma()) {
            skipToken();
            constDecl->constDefs.push_back(_tryConstDef());
        }

        ConstDecl::print(_correctOutput);
        return constDecl;
    }

    u_ptr<Ident> Parser1::_tryIdent()  {
        auto ident = std::make_unique<Ident>(peekToken());
        assert(peekToken().getType() == TK_IDENFR);
        ident->str = getToken().getContent();
        Ident::print(_correctOutput);
        return ident;
    }

    u_ptr<InitVal> Parser1::_tryInitVal()  {
        auto initVal = std::make_unique<InitVal>(peekToken());

        Token && t1 = peekToken();
        // try stringConst
        if (t1.getType() == TK_STRCON) {
            initVal->stringConst = _tryStringConst();
        }
        else if (t1.getType() == TK_LBRACE) {
            skipToken();
            auto exp = _tryExp();
            initVal->addExp(exp);

            while (t1.isComma()) {
                skipToken();
                exp = _tryExp();
                initVal->addExp(exp);
            }

            _matchCurToken(TK_RBRACE);
        }
        else {
            // ConstExp;
            auto exp = _tryExp();
            initVal->addExp(exp);
        }
        InitVal::print(_correctOutput);
        InitVal::print(_correctOutput);
        return initVal;
    }

    u_ptr<VarDef> Parser1::_tryVarDef()  {
        auto varDef = std::make_unique<VarDef>(peekToken());
        assert(peekToken().getType() == TK_IDENFR);
        varDef->ident = _tryIdent();

        if (peekToken().getType() == TK_LBRACK) {
            skipToken();
            varDef->constExp = _tryConstExp();
            _matchCurToken(TK_RBRACK);
        }

        if (peekToken().getType() == TK_ASSIGN) {
            skipToken();
            varDef->initVal = _tryInitVal();
        }

        VarDef::print(_correctOutput);
        return varDef;
    }

    u_ptr<VarDecl> Parser1::_tryVarDecl()  {
        auto varDecl = std::make_unique<VarDecl>(peekToken());
        Token && t1 = peekToken();
        Token&& t2 = peekToken();
        assert(t1.isBType());
        varDecl->bType = _tryBType();

        varDecl->varDefs.push_back(_tryVarDef());

        while (peekToken().isComma()) {
            varDecl->varDefs.push_back(_tryVarDef());
        }

        _matchCurToken(TK_SEMICN);

        VarDecl::print(_correctOutput);
        return varDecl;
    }

    u_ptr<FuncType> Parser1::_tryFuncType()  {
        auto funcType = std::make_unique<FuncType>(peekToken());

        Token&& t1 = peekToken();
        assert(t1.isFuncType());
        skipToken();

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
            assert(0);
        }

        FuncType::print(_correctOutput);
        return funcType;
    }

    u_ptr<FuncFParam> Parser1::_tryFuncFParam()  {
        auto funcFParam = std::make_unique<FuncFParam>(peekToken());

        assert(peekToken().isBType());
        funcFParam->bType = _tryBType();
        assert(peekToken().getType() == TK_IDENFR);
        funcFParam->ident = _tryIdent();

        if (peekToken().getType() == TK_LBRACK) {
            skipToken();
            funcFParam->isArray = true;
            _matchCurToken(TK_RBRACK);
        }

        FuncFParam::print(_correctOutput);
        return funcFParam;
    }

    u_ptr<FuncFParams> Parser1::_tryFuncFParams()  {
        auto funcFParams = std::make_unique<FuncFParams>(peekToken());

        assert(peekToken().isBType());
        funcFParams->funcFParams.push_back(_tryFuncFParam());
        while (peekToken().isComma()) {
            funcFParams->funcFParams.push_back(_tryFuncFParam());
        }
        FuncFParams::print(_correctOutput);
        return funcFParams;
    }

    u_ptr<IntConst> Parser1::_tryIntConst()  {
        auto intConst = std::make_unique<IntConst>(peekToken());

        assert(peekToken().getType() == TK_INTCON);
        intConst->val = std::stoi(getToken().getContent());

        IntConst::print(_correctOutput);
        return intConst;
    }

    u_ptr<Number> Parser1::_tryNumber()  {
        auto number = std::make_unique<Number>(peekToken());

        assert(peekToken().getType() == TK_INTCON);
        number->intConst = _tryIntConst();

        Number::print(_correctOutput);
        return number;
    }

    u_ptr<CharConst> Parser1::_tryCharConst()  {
        auto charConst = std::make_unique<CharConst>(peekToken());

        assert(peekToken().getType() == TK_CHRCON);
        charConst->ch = getToken().CHRCONToChar();

        CharConst::print(_correctOutput);
        return charConst;
    }

    u_ptr<Character> Parser1::_tryCharacter()  {
        auto character = std::make_unique<Character>(peekToken());

        assert(peekToken().getType() == TK_CHRCON);
        character->charConst = _tryCharConst();

        Character::print(_correctOutput);
        return character;
    }

    u_ptr<PrimaryExp> Parser1::_tryPrimaryExp()  {
        auto primaryExp = std::make_unique<PrimaryExp>(peekToken());

        Token&& t1 = peekToken();
        if (t1.getType() == TK_INTCON) {
            // try number
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(_tryNumber());
        }
        else if (t1.getType() == TK_CHRCON) {
            // try Character
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(_tryCharacter());
        }
        else if (t1.getType() == TK_LPARENT) {
            // try (exp)
            skipToken();
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(_tryExp());
            _matchCurToken(TK_RPARENT);
        }
        else if (t1.getType() == TK_IDENFR) {
            // try LVal
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(_tryLVal());
        }
        else {
            assert(0);
        }

        PrimaryExp::print(_correctOutput);
        return primaryExp;
    }

    u_ptr<PrimaryExp> Parser1::_tryPrimaryExp(u_ptr<LVal>& lVal)  {
        auto primaryExp = std::make_unique<PrimaryExp>(peekToken());

        primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(std::move(lVal));

        PrimaryExp::print(_correctOutput);
        return primaryExp;
    }

    u_ptr<FuncRParams> Parser1::_tryFuncRParams()  {
        auto funcRParams = std::make_unique<FuncRParams>(peekToken());

        // !!! NO ASSERT!!!
        funcRParams->exps.push_back(_tryExp());
        while (peekToken().isComma()) {
            skipToken();
            funcRParams->exps.push_back(_tryExp());
        }

        FuncRParams::print(_correctOutput);
        return funcRParams;
    }

    u_ptr<UnaryOp> Parser1::_tryUnaryOp()  {
        auto unaryOp = std::make_unique<UnaryOp>(peekToken());

        assert(peekToken().isUnaryOp());
        Token&& t1 = getToken();
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
            assert(0);
        }

        UnaryOp::print(_correctOutput);
        return unaryOp;
    }

    u_ptr<FuncCall> Parser1::_tryFuncCall()  {
        auto funcCall = std::make_unique<FuncCall>(peekToken());

        assert(peekToken().getType() == TK_IDENFR);
        funcCall->ident = _tryIdent();

        _matchCurToken(TK_LPARENT);
        if (peekToken().isBType()) {
            funcCall->funcRParams = _tryFuncRParams();
        }
        _matchCurToken(TK_RPARENT);

        FuncCall::print(_correctOutput);
        return funcCall;
    }

    u_ptr<UnaryExp> Parser1::_tryUnaryExp()  {
        auto unaryExp = std::make_unique<UnaryExp>(peekToken());

        // !!! NO ASSERT !!!
        while (peekToken().isUnaryOp()) {
            unaryExp->unaryOps.push_back(_tryUnaryOp());
        }
        if (peekToken().getType() == TK_IDENFR &&
            peekToken(1).getType() == TK_LPARENT) {
            unaryExp->unaryExp = std::make_unique<UnaryExpVariant>(_tryFuncCall());
        }
        else {
            unaryExp->unaryExp = std::make_unique<UnaryExpVariant>(_tryPrimaryExp());
        }

        UnaryExp::print(_correctOutput);
        return unaryExp;
    }

    u_ptr<UnaryExp> Parser1::_tryUnaryExp(u_ptr<LVal>& lVal)  {
        auto unaryExp = std::make_unique<UnaryExp>(peekToken());

        // !!! NO ASSERT !!!
        unaryExp->unaryExp = std::make_unique<UnaryExpVariant>(_tryPrimaryExp(lVal));

        UnaryExp::print(_correctOutput);
        return unaryExp;
    }

    u_ptr<MulExp> Parser1::_tryMulExp()  {
        auto mulExp = std::make_unique<MulExp>(peekToken());

        // !!! NO ASSERT !!!
        mulExp->unaryExps.push_back(_tryUnaryExp());
        while (peekToken().isMulExpOp()) {
            mulExp->ops.push_back(getToken());
            mulExp->unaryExps.push_back(_tryUnaryExp());
        }

        MulExp::print(_correctOutput);
        return mulExp;
    }

    u_ptr<MulExp> Parser1::_tryMulExp(u_ptr<LVal>& lVal)  {
        auto mulExp = std::make_unique<MulExp>(peekToken());

        // !!! NO ASSERT !!!
        mulExp->unaryExps.push_back(_tryUnaryExp(lVal));
        while (peekToken().isMulExpOp()) {
            mulExp->ops.push_back(getToken());
            mulExp->unaryExps.push_back(_tryUnaryExp());
        }

        MulExp::print(_correctOutput);
        return mulExp;
    }

    u_ptr<AddExp> Parser1::_tryAddExp()  {
        auto addExp = std::make_unique<AddExp>(peekToken());

        // !!! NO ASSERT !!!
        addExp->mulExps.push_back(_tryMulExp());
        while (peekToken().isAddExpOp()) {
            addExp->ops.push_back(getToken());
            addExp->mulExps.push_back(_tryMulExp());
        }

        AddExp::print(_correctOutput);
        return addExp;
    }

    u_ptr<AddExp> Parser1::_tryAddExp(u_ptr<LVal>& lVal)  {
        auto addExp = std::make_unique<AddExp>(peekToken());

        // !!! NO ASSERT !!!
        addExp->mulExps.push_back(_tryMulExp(lVal));
        while (peekToken().isAddExpOp()) {
            addExp->ops.push_back(getToken());
            addExp->mulExps.push_back(_tryMulExp());
        }

        AddExp::print(_correctOutput);
        return addExp;
    }

    u_ptr<Exp> Parser1::_tryExp()  {
        auto exp = std::make_unique<Exp>(peekToken());

        // !!! NO ASSERT !!!
        exp->addExp = _tryAddExp();

        Exp::print(_correctOutput);
        return exp;
    }

    u_ptr<Exp> Parser1::_tryExp(u_ptr<LVal>& lVal) {
        auto exp = std::make_unique<Exp>(peekToken());

        // !!! NO ASSERT !!!
        exp->addExp = _tryAddExp(lVal);

        Exp::print(_correctOutput);
        return exp;
    }

    u_ptr<LVal> Parser1::_tryLVal()  {
        auto lVal = std::make_unique<LVal>(peekToken());

        assert(peekToken().getType() == TK_IDENFR);
        lVal->ident = _tryIdent();
        if (peekToken().getType() == TK_LBRACK) {
            skipToken();
            lVal->exp = _tryExp();
            _matchCurToken(TK_RBRACK);
        }
        else {
            lVal->exp = nullptr;
        }

        LVal::print(_correctOutput);
        return lVal;
    }

    u_ptr<RelExp> Parser1::_tryRelExp()  {
        auto relExp = std::make_unique<RelExp>(peekToken());

        // !!! NO ASSERT !!!
        relExp->addExps.push_back(_tryAddExp());
        while (peekToken().isRelExpOp()) {
            relExp->ops.push_back(getToken());
            relExp->addExps.push_back(_tryAddExp());
        }

        RelExp::print(_correctOutput);
        return relExp;
    }

    u_ptr<EqExp> Parser1::_tryEqExp()  {
        auto eqExp = std::make_unique<EqExp>(peekToken());

        // !!! NO ASSERT !!!
        eqExp->relExps.push_back(_tryRelExp());
        while (peekToken().isEqExpOp()) {
            eqExp->ops.push_back(getToken());
            eqExp->relExps.push_back(_tryRelExp());
        }

        EqExp::print(_correctOutput);
        return eqExp;
    }

    u_ptr<LAndExp> Parser1::_tryLAndExp()  {
        auto lAndExp = std::make_unique<LAndExp>(peekToken());

        // !!! NO ASSERT !!!
        lAndExp->eqExps.push_back(_tryEqExp());
        while (peekToken().getType() == TK_AND) {
            skipToken();
            lAndExp->eqExps.push_back(_tryEqExp());
        }

        LAndExp::print(_correctOutput);
        return lAndExp;
    }

    u_ptr<LOrExp> Parser1::_tryLOrExp()  {
        auto lOrExp = std::make_unique<LOrExp>(peekToken());

        // !!! NO ASSERT !!!
        lOrExp->lAndExps.push_back(_tryLAndExp());
        while (peekToken().getType() == TK_OR) {
            skipToken();
            lOrExp->lAndExps.push_back(_tryLAndExp());
        }

        LOrExp::print(_correctOutput);
        return lOrExp;
    }

    u_ptr<Cond> Parser1::_tryCond()  {
        auto cond = std::make_unique<Cond>(peekToken());

        // !!! NO ASSERT !!!
        cond->lOrExp = _tryLOrExp();

        Cond::print(_correctOutput);
        return cond;
    }

    u_ptr<Assignment> Parser1::_tryAssignment()  {
        auto assignment = std::make_unique<Assignment>(peekToken());

        assert(peekToken().getType() == TK_IDENFR);
        assignment->lVal = _tryLVal();
        _matchCurToken(TK_ASSIGN);
        assignment->exp = _tryExp();

        Assignment::print(_correctOutput);
        return assignment;
    }

    u_ptr<ForStmt> Parser1::_tryForStmt()  {
        auto forStmt = std::make_unique<ForStmt>(peekToken());

        assert(peekToken().getType() == TK_FORTK);
        _matchCurToken(TK_FORTK);
        _matchCurToken(TK_LPARENT);

        if (peekToken().getType() == TK_IDENFR) {
            forStmt->init = _tryAssignment();
        }
        _matchCurToken(TK_COMMA);

        forStmt->cond = _tryCond();
        _matchCurToken(TK_COMMA);

        if (peekToken().getType() == TK_IDENFR) {
            forStmt->update = _tryAssignment();
        }
        _matchCurToken(TK_RPARENT);

        forStmt->stmt = _tryStmt();

        ForStmt::print(_correctOutput);
        return forStmt;
    }

    u_ptr<AssignStmt> Parser1::_tryAssignStmt()  {
        auto assignStmt = std::make_unique<AssignStmt>(peekToken());

        assert(peekToken().getType() == TK_IDENFR);

        assignStmt->lVal = _tryLVal();
        _matchCurToken(TK_ASSIGN);
        assignStmt->exp = _tryExp();
        _matchCurToken(TK_SEMICN);

        AssignStmt::print(_correctOutput);
        return assignStmt;
    }

    u_ptr<AssignStmt> Parser1::_tryAssignStmt(u_ptr<LVal>& lVal)  {
        auto assignStmt = std::make_unique<AssignStmt>(peekToken());

        assignStmt->lVal = std::move(lVal);
        _matchCurToken(TK_ASSIGN);
        assignStmt->exp = _tryExp();
        _matchCurToken(TK_SEMICN);

        AssignStmt::print(_correctOutput);
        return assignStmt;
    }

    u_ptr<IfStmt> Parser1::_tryIfStmt()  {
        auto ifStmt = std::make_unique<IfStmt>(peekToken());

        assert(peekToken().getType() == TK_IFTK);
        skipToken();
        _matchCurToken(TK_LPARENT);
        ifStmt->cond = _tryCond();
        _matchCurToken(TK_RPARENT);
        ifStmt->ifStmt = _tryStmt();

        if (peekToken().getType() == TK_ELSETK) {
            skipToken();
            ifStmt->elseStmt = _tryStmt();
        }

        IfStmt::print(_correctOutput);
        return ifStmt;
    }

    u_ptr<BreakStmt> Parser1::_tryBreakStmt()  {
        auto breakStmt = std::make_unique<BreakStmt>(peekToken());

        assert(peekToken().getType() == TK_BREAKTK);
        skipToken();
        _matchCurToken(TK_SEMICN);

        BreakStmt::print(_correctOutput);
        return breakStmt;
    }

    u_ptr<ContinueStmt> Parser1::_tryContinueStmt()  {
        auto continueStmt = std::make_unique<ContinueStmt>(peekToken());

        assert(peekToken().getType() == TK_CONTINUETK);
        skipToken();
        _matchCurToken(TK_SEMICN);

        ContinueStmt::print(_correctOutput);
        return continueStmt;
    }

    u_ptr<ReturnStmt> Parser1::_tryReturnStmt()  {
        auto returnStmt = std::make_unique<ReturnStmt>(peekToken());

        assert(peekToken().getType() == TK_RETURNTK);
        skipToken();
        if (peekToken().isExpFirst()) {
            returnStmt->exp = _tryExp();
        }
        _matchCurToken(TK_SEMICN);

        ReturnStmt::print(_correctOutput);
        return returnStmt;
    }

    u_ptr<GetintStmt> Parser1::_tryGetintStmt()  {
        auto getintStmt = std::make_unique<GetintStmt>(peekToken());

        assert(peekToken().getType() == TK_IDENFR);
        getintStmt->lVal = _tryLVal();
        _matchCurToken(TK_ASSIGN);
        _matchCurToken(TK_GETINTTK);
        _matchCurToken(TK_LPARENT);
        _matchCurToken(TK_RPARENT);
        _matchCurToken(TK_SEMICN);

        GetintStmt::print(_correctOutput);
        return getintStmt;
    }

    u_ptr<GetintStmt> Parser1::_tryGetintStmt(u_ptr<LVal>& lVal)  {
        auto getintStmt = std::make_unique<GetintStmt>(peekToken());

        getintStmt->lVal = std::move(lVal);
        _matchCurToken(TK_ASSIGN);
        _matchCurToken(TK_GETINTTK);
        _matchCurToken(TK_LPARENT);
        _matchCurToken(TK_RPARENT);
        _matchCurToken(TK_SEMICN);

        GetintStmt::print(_correctOutput);
        return getintStmt;
    }

    u_ptr<GetcharStmt> Parser1::_tryGetcharStmt()  {
        auto getcharStmt = std::make_unique<GetcharStmt>(peekToken());

        assert(peekToken().getType() == TK_IDENFR);
        getcharStmt->lVal = _tryLVal();
        _matchCurToken(TK_ASSIGN);
        _matchCurToken(TK_GETCHARTK);
        _matchCurToken(TK_LPARENT);
        _matchCurToken(TK_RPARENT);
        _matchCurToken(TK_SEMICN);

        GetcharStmt::print(_correctOutput);
        return getcharStmt;
    }

    u_ptr<GetcharStmt> Parser1::_tryGetcharStmt(u_ptr<LVal>& lVal)  {
        auto getcharStmt = std::make_unique<GetcharStmt>(peekToken());

        getcharStmt->lVal = std::move(lVal);
        _matchCurToken(TK_ASSIGN);
        _matchCurToken(TK_GETCHARTK);
        _matchCurToken(TK_LPARENT);
        _matchCurToken(TK_RPARENT);
        _matchCurToken(TK_SEMICN);

        GetcharStmt::print(_correctOutput);
        return getcharStmt;
    }

    u_ptr<PrintfStmt> Parser1::_tryPrintfStmt()  {
        auto printfStmt = std::make_unique<PrintfStmt>(peekToken());

        assert(peekToken().getType() == TK_PRINTFTK);
        skipToken();
        _matchCurToken(TK_LPARENT);
        assert(peekToken().getType() == TK_STRCON);
        printfStmt->stringConst = _tryStringConst();
        Token&& t1 = peekToken();
        while (peekToken().isComma()) {
            skipToken();
            printfStmt->exps.push_back(_tryExp());
        }
        _matchCurToken(TK_RPARENT);
        _matchCurToken(TK_SEMICN);

        PrintfStmt::print(_correctOutput);
        return printfStmt;
    }

    u_ptr<Stmt> Parser1::_tryStmt()  {
        auto stmt = std::make_unique<Stmt>(peekToken());

        Token&& t1 = peekToken(0);
        Token&& t2 = peekToken(1);
        Token&& t3 = peekToken(2);

        if (t1.getType() == TK_IFTK) {
            // try if
            stmt->stmt = std::make_unique<StmtVariant>(_tryIfStmt());
        }
        else if (t1.getType() == TK_FORTK) {
            // try for
            stmt->stmt = std::make_unique<StmtVariant>(_tryForStmt());
        }
        else if (t1.getType() == TK_BREAKTK) {
            // try break
            stmt->stmt = std::make_unique<StmtVariant>(_tryBreakStmt());
        }
        else if (t1.getType() == TK_CONTINUETK) {
            // try continue
            stmt->stmt = std::make_unique<StmtVariant>(_tryContinueStmt());
        }
        else if (t1.getType() == TK_RETURNTK) {
            // try return
            stmt->stmt = std::make_unique<StmtVariant>(_tryReturnStmt());
        }
        else if (t1.getType() == TK_PRINTFTK) {
            // try printf
            stmt->stmt = std::make_unique<StmtVariant>(_tryPrintfStmt());
        }
        else if (t1.getType() == TK_LBRACE) {
            // try block
            stmt->stmt = std::make_unique<StmtVariant>(_tryBlock());
        }
        else if (t1.getType() == TK_SEMICN) {
            skipToken();
            stmt->stmt = nullptr;
        }
        else if (t1.isUnaryOp() || t1.getType() == TK_LPARENT ||
                 t1.getType() == TK_INTCON || t1.getType() == TK_CHARTK ||
                 (t1.getType() == TK_IDENFR && t2.getType() == TK_LPARENT)) {
            // Exp
            stmt->stmt = std::make_unique<StmtVariant>(_tryExp());
            _matchCurToken(TK_SEMICN);
        }
        else if (t1.getType() == TK_IDENFR) {
            // this could be LVal [^=] and thus a primary exp and thus an exp
            // it could also be LVal = and thus not an exp
            // thus we need to parse LVal once and then check;
            auto lVal = _tryLVal();
            t1 = peekToken(0);
            t2 = peekToken(1);
            if (t1.getType() != TK_ASSIGN) {
                // ExpStmt
                stmt->stmt = std::make_unique<StmtVariant>(_tryExp(lVal));
            }
            else if (t2.getType() == TK_GETINTTK) {
                // getintStmt
                stmt->stmt = std::make_unique<StmtVariant>(_tryGetintStmt(lVal));
            }
            else if (t2.getType() == TK_GETCHARTK) {
                // getcharStmt
                stmt->stmt = std::make_unique<StmtVariant>(_tryGetcharStmt(lVal));
            }
            else {
                // this is an assignStmt;
                stmt->stmt = std::make_unique<StmtVariant>(_tryAssignStmt(lVal));
            }
        }
        Stmt::print(_correctOutput);
        return stmt;
    }

    u_ptr<BlockItem> Parser1::_tryBlockItem()  {
        auto blockItem = std::make_unique<BlockItem>(peekToken());

        while (peekToken().getType() != TK_RBRACE) {
            if (peekToken().isBType()) {
                blockItem->blockItem = std::make_unique<BlockItemVariant>(_tryDecl());
            }
            else {
                blockItem->blockItem = std::make_unique<BlockItemVariant>(_tryStmt());
            }
        }

        BlockItem::print(_correctOutput);
        return blockItem;
    }

    u_ptr<Block> Parser1::_tryBlock()  {
        auto block = std::make_unique<Block>(peekToken());

        assert(peekToken().getType() == TK_LBRACE);
        skipToken();
        block->blockItem = _tryBlockItem();
        _matchCurToken(TK_RBRACE);

        Block::print(_correctOutput);
        return block;
    }

    u_ptr<FuncDef> Parser1::_tryFuncDef()  {
        auto funcDef = std::make_unique<FuncDef>(peekToken());

        assert(peekToken().isFuncType());
        funcDef->funcType = _tryFuncType();
        funcDef->ident = _tryIdent();
        _matchCurToken(TK_LPARENT);
        if (peekToken().isBType()) {
            funcDef->funcFParams = _tryFuncFParams();
        }
        _matchCurToken(TK_RPARENT);
        funcDef->block = _tryBlock();

        FuncDef::print(_correctOutput);
        return funcDef;
    }

    u_ptr<Decl> Parser1::_tryDecl()  {
        auto decl = std::make_unique<Decl>(peekToken());

        // !!! NO ASSERT !!!
        if (peekToken().getType() == TK_CONSTTK) {
            decl->decl = std::make_unique<DeclVariant>(_tryConstDecl());
        }
        else {
            decl->decl = std::make_unique<DeclVariant>(_tryVarDecl());
        }

        Decl::print(_correctOutput);
        return decl;
    }

    u_ptr<MainFuncDef> Parser1::_tryMainFuncDef()  {
        auto mainFuncDef = std::make_unique<MainFuncDef>(peekToken());

        assert(peekToken().getType() == TK_INTTK);
        assert(peekToken(1).getType() == TK_MAINTK);
        skipToken();
        skipToken();
        _matchCurToken(TK_LPARENT);
        _matchCurToken(TK_RPARENT);
        mainFuncDef->block = _tryBlock();

        MainFuncDef::print(_correctOutput);
        return mainFuncDef;
    }

    u_ptr<CompUnit> Parser1::_tryCompUnit()  {
        auto compUnit = std::make_unique<CompUnit>(peekToken());

        while (1) {
            Token&& t1 = peekToken(0);
            Token&& t2 = peekToken(1);
            Token&& t3 = peekToken(2);
            Token&& t4 = peekToken(3);
            if (t1.getType() == TK_CONSTTK || 
                t1.isBType() && t2.getType() == TK_IDENFR && t3.getType() != TK_LPARENT) {
                compUnit->decls.push_back(_tryDecl());
            }
            else {
                break;
            }
        }

        while (1) {
            Token&& t1 = peekToken(0);
            Token&& t2 = peekToken(1);
            Token&& t3 = peekToken(2);
            if (t1.isFuncType() && t2.getType() == TK_IDENFR && t3.getType() == TK_LPARENT) {
                compUnit->funcDefs.push_back(_tryFuncDef());
            }
            else {
                break;
            }
        }

        assert(peekToken().getType() == TK_INTTK);
        assert(peekToken(1).getType() == TK_MAINTK);
        compUnit->mainFuncDef = _tryMainFuncDef();
        CompUnit::print(_correctOutput);
        return compUnit;
    }

    u_ptr<CompUnit> Parser1::parse() {
        return _tryCompUnit();
    }

    bool Parser1::_matchCurToken(const TokenType expectType) {
        return _match(peekToken(), expectType);
    }

    bool Parser1::_match(Token&& t, const TokenType expectType) const {
        if (t.getType() != expectType) {
            if (expectType == TK_SEMICN) {
                _reporter.report(t.getLin(), 'i');
            }
            else if (expectType == TK_RPARENT) {
                _reporter.report(t.getLin(), 'j');
            }
            else if (expectType == TK_RBRACK) {
                _reporter.report(t.getLin(), 'k');
            }
            return false;
        }
        skipToken();
        return true;
    }

};