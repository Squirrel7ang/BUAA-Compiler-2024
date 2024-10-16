//
// Created by tang on 10/16/24.
//

#include "parser1.hpp"

#include <cassert>

#include "ast.hpp"

namespace tang {

    template <>
    auto Parser1::_try<BType>() -> u_ptr<BType> {
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
        return bType;
    }

    template <>
    auto Parser1::_try<ConstExp>() -> u_ptr<ConstExp> {
        auto constExp = std::make_unique<ConstExp>(peekToken());
        constExp->addExp = _try<AddExp>();
        return constExp;
    }

    template <>
    auto Parser1::_try<StringConst>() -> u_ptr<StringConst> {
        auto stringConst = std::make_unique<StringConst>(peekToken());
        assert(peekToken().getType() == TK_STRCON);

        std::string&& _content = peekToken().getContent();
        std::string&& content = _content.substr(1, _content.length() - 2);
        stringConst->str = std::make_unique<std::string>(content);

        return stringConst;
    }

    template <>
    auto Parser1::_try<ConstInitVal>() -> u_ptr<ConstInitVal> {
        auto constInitVal = std::make_unique<ConstInitVal>(peekToken());

        Token && t1 = peekToken();
        // try stringConst
        if (t1.getType() == TK_STRCON) {
            constInitVal->stringConst = _try<StringConst>();
        }
        else if (t1.getType() == TK_LBRACE) {
            skipToken();
            auto constExp = _try<ConstExp>();
            constInitVal->addConstExp(constExp);

            while (t1.isComma()) {
                skipToken();
                constExp = _try<ConstExp>();
                constInitVal->addConstExp(constExp);
            }

            _matchCurToken(TK_RBRACE);
        }
        else {
            // ConstExp;
            auto constExp = _try<ConstExp>();
            constInitVal->addConstExp(constExp);
        }

        return constInitVal;
    }

    template <>
    auto Parser1::_try<ConstDef>() -> u_ptr<ConstDef> {
        auto constDef = std::make_unique<ConstDef>(peekToken());

        assert(peekToken().getType() == TK_IDENFR);

        constDef->ident = _try<Ident>();
        if (peekToken().getType() == TK_LBRACK) {
            skipToken();
            constDef->constExp = _try<ConstExp>();

            _matchCurToken(TK_RBRACE);
        }
        _matchCurToken(TK_ASSIGN);
        constDef->constInitVal = _try<ConstInitVal>();

        return constDef;
    }

    template <>
    auto Parser1::_try<ConstDecl>() -> u_ptr<ConstDecl> {
        auto constDecl = std::make_unique<ConstDecl>(peekToken());
        assert(peekToken().getType() == TK_CONSTTK);

        _matchCurToken(TK_CONSTTK);
        constDecl->bType = _try<BType>();
        constDecl->constDefs.push_back(_try<ConstDef>());
        while (peekToken().isComma()) {
            skipToken();
            constDecl->constDefs.push_back(_try<ConstDef>());
        }

        return constDecl;
    }

    template <>
    auto Parser1::_try<Ident>() -> u_ptr<Ident> {
        auto ident = std::make_unique<Ident>(peekToken());
        assert(peekToken().getType() == TK_IDENFR);
        ident->str = getToken().getContent();
        return ident;
    }

    template <>
    auto Parser1::_try<InitVal>() -> u_ptr<InitVal> {
        auto initVal = std::make_unique<InitVal>(peekToken());

        Token && t1 = peekToken();
        // try stringConst
        if (t1.getType() == TK_STRCON) {
            initVal->stringConst = _try<StringConst>();
        }
        else if (t1.getType() == TK_LBRACE) {
            skipToken();
            auto exp = _try<exp>();
            initVal->addExp(exp);

            while (t1.isComma()) {
                skipToken();
                exp = _try<exp>();
                initVal->addExp(exp);
            }

            _matchCurToken(TK_RBRACE);
        }
        else {
            // ConstExp;
            auto exp = _try<exp>();
            initVal->addExp(exp);
        }
        return initVal;
    }

    template <>
    auto Parser1::_try<VarDef>() -> u_ptr<VarDef> {
        auto varDef = std::make_unique<VarDef>(peekToken());
        assert(peekToken().getType() == TK_IDENFR);
        varDef->ident = _try<Ident>();

        if (peekToken().getType() == TK_LBRACK) {
            skipToken();
            varDef->constExp = _try<ConstExp>();
            _matchCurToken(TK_RBRACK);
        }

        if (peekToken().getType() == TK_ASSIGN) {
            skipToken();
            varDef->initVal = _try<InitVal>();
        }

        return varDef;
    }

    template <>
    auto Parser1::_try<VarDecl>() -> u_ptr<VarDecl> {
        auto varDecl = std::make_unique<VarDecl>(peekToken());
        assert(peekToken().isBType());
        varDecl->bType = _try<BType>();

        varDecl->varDefs.push_back(_try<VarDef>());

        while (peekToken().isComma()) {
            varDecl->varDefs.push_back(_try<VarDef>());
        }

        _matchCurToken(TK_SEMICN);

        return varDecl;
    }

    template <>
    auto Parser1::_try<FuncType>() -> u_ptr<FuncType> {
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

        return funcType;
    }

    template <>
    auto Parser1::_try<FuncFParam>() -> u_ptr<FuncFParam> {
        auto funcFParam = std::make_unique<FuncFParam>(peekToken());

        assert(peekToken().isBType());
        funcFParam->bType = _try<BType>();
        assert(peekToken().getType() == TK_IDENFR);
        funcFParam->ident = _try<Ident>();

        if (peekToken().getType() == TK_LBRACK) {
            skipToken();
            funcFParam->isArray = true;
            _matchCurToken(TK_RBRACK);
        }

        return funcFParam;
    }

    template <>
    auto Parser1::_try<FuncFParams>() -> u_ptr<FuncFParams> {
        auto funcFParams = std::make_unique<FuncFParams>(peekToken());

        assert(peekToken().isBType());
        funcFParams->funcFParams.push_back(_try<FuncFParam>());
        while (peekToken().isComma()) {
            funcFParams->funcFParams.push_back(_try<FuncFParam>());
        }
        return funcFParams;
    }

    template <>
    auto Parser1::_try<IntConst>() -> u_ptr<IntConst> {
        auto intConst = std::make_unique<IntConst>(peekToken());

        assert(peekToken().getType() == TK_INTCON);
        intConst->val = std::stoi(getToken().getContent());

        return intConst;
    }

    template <>
    auto Parser1::_try<Number>() -> u_ptr<Number> {
        auto number = std::make_unique<Number>(peekToken());

        assert(peekToken().getType() == TK_INTCON);
        number->intConst = _try<IntConst>();

        return number;
    }

    template <>
    auto Parser1::_try<CharConst>() -> u_ptr<CharConst> {
        auto charConst = std::make_unique<CharConst>(peekToken());

        assert(peekToken().getType() == TK_CHRCON);
        charConst->ch = getToken().getContent()[1];

        return charConst;
    }

    template <>
    auto Parser1::_try<Character>() -> u_ptr<Character> {
        auto character = std::make_unique<Character>(peekToken());

        assert(peekToken().getType() == TK_CHRCON);
        character->charConst = _try<CharConst>();

        return character;
    }

    template <>
    auto Parser1::_try<PrimaryExp>() -> u_ptr<PrimaryExp> {
        auto primaryExp = std::make_unique<PrimaryExp>(peekToken());

        Token&& t1 = peekToken();
        if (t1.getType() == TK_INTCON) {
            // try number
            auto number = _try<Number>();
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(std::move(number));
        }
        else if (t1.getType() == TK_CHRCON) {
            // try Character
            auto character = _try<Character>();
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(std::move(character));
        }
        else if (t1.getType() == TK_LPARENT) {
            // try (exp)
            skipToken();
            auto exp = _try<Exp>();
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(std::move(exp));
            _matchCurToken(TK_RPARENT);
        }
        else if (t1.getType() == TK_IDENFR) {
            // try LVal
            auto lVal = _try<LVal>();
            primaryExp->primaryExp = std::make_unique<PrimaryExpVariant>(std::move(lVal));
        }
        else {
            assert(0);
        }

        return primaryExp;
    }

    template <>
    auto Parser1::_try<FuncRParams>() -> u_ptr<FuncRParams> {
        auto funcRParams = std::make_unique<FuncRParams>(peekToken());

        // !!! NO ASSERT!!!
        funcRParams->exps.push_back(_try<Exp>());
        while (peekToken().isComma()) {
            skipToken();
            funcRParams->exps.push_back(_try<Exp>());
        }

        return funcRParams;
    }

    template <>
    auto Parser1::_try<UnaryOp>() -> u_ptr<UnaryOp> {
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

        return unaryOp;
    }

    template <>
    auto Parser1::_try<FuncCall>() -> u_ptr<FuncCall> {
        auto funcCall = std::make_unique<FuncCall>(peekToken());

        assert(peekToken().getType() == TK_IDENFR);
        funcCall->ident = _try<Ident>();

        _matchCurToken(TK_LPARENT);
        if (peekToken().isBType()) {
            funcCall->funcRParams = _try<FuncRParams>();
        }
        _matchCurToken(TK_RPARENT);

        return funcCall;
    }

    template <>
    auto Parser1::_try<UnaryExp>() -> u_ptr<UnaryExp> {
        auto unaryExp = std::make_unique<UnaryExp>(peekToken());

        // !!! NO ASSERT !!!
        while (peekToken().isUnaryOp()) {
            unaryExp->unaryOps.push_back(_try<UnaryOp>());
        }
        if (peekToken().getType() == TK_IDENFR &&
            peekToken(1).getType() == TK_LPARENT) {
            unaryExp->unaryExp = std::make_unique<UnaryExpVariant>(_try<FuncCall>());
        }
        else {
            unaryExp->unaryExp = std::make_unique<UnaryExpVariant>(_try<PrimaryExp>());
        }

        return unaryExp;
    }

    template <>
    auto Parser1::_try<MulExp>() -> u_ptr<MulExp> {
        auto mulExp = std::make_unique<MulExp>(peekToken());

        // !!! NO ASSERT !!!
        mulExp->unaryExps.push_back(_try<UnaryExp>());
        while (peekToken().isMulExpOp()) {
            mulExp->ops.push_back(getToken());
            mulExp->unaryExps.push_back(_try<UnaryExp>());
        }

        return mulExp;
    }

    template <>
    auto Parser1::_try<AddExp>() -> u_ptr<AddExp> {
        auto addExp = std::make_unique<AddExp>(peekToken());

        // !!! NO ASSERT !!!
        addExp->mulExps.push_back(_try<MulExp>());
        while (peekToken().isAddExpOp()) {
            addExp->ops.push_back(getToken());
            addExp->mulExps.push_back(_try<MulExp>());
        }

        return addExp;
    }

    template <>
    auto Parser1::_try<Exp>() -> u_ptr<Exp> {
        auto exp = std::make_unique<Exp>(peekToken());

        // !!! NO ASSERT !!!
        exp->addExp = _try<AddExp>();

        return exp;
    }

    template <>
    auto Parser1::_try<LVal>() -> u_ptr<LVal> {
        auto lVal = std::make_unique<LVal>(peekToken());

        assert(peekToken().getType() == TK_IDENFR);
        lVal->ident = _try<Ident>();
        if (peekToken().getType() == TK_LBRACK) {
            skipToken();
            lVal->exp = _try<Exp>();
            _matchCurToken(TK_RBRACK);
        }
        else {
            lVal->exp = nullptr;
        }

        return lVal;
    }

    template <>
    auto Parser1::_try<RelExp>() -> u_ptr<RelExp> {
        auto relExp = std::make_unique<RelExp>(peekToken());

        // !!! NO ASSERT !!!
        relExp->addExps.push_back(_try<AddExp>());
        while (peekToken().isRelExpOp()) {
            relExp->ops.push_back(getToken());
            relExp->addExps.push_back(_try<AddExp>());
        }

        return relExp;
    }

    template <>
    auto Parser1::_try<EqExp>() -> u_ptr<EqExp> {
        auto eqExp = std::make_unique<EqExp>(peekToken());

        // !!! NO ASSERT !!!
        eqExp->relExps.push_back(_try<RelExp>());
        while (peekToken().isEqExpOp()) {
            eqExp->ops.push_back(getToken());
            eqExp->relExps.push_back(_try<RelExp>());
        }

        return eqExp;
    }

    template <>
    auto Parser1::_try<LAndExp>() -> u_ptr<LAndExp> {
        auto lAndExp = std::make_unique<LAndExp>(peekToken());

        // !!! NO ASSERT !!!
        lAndExp->eqExps.push_back(_try<EqExp>());
        while (peekToken().getType() == TK_AND) {
            skipToken();
            lAndExp->eqExps.push_back(_try<EqExp>());
        }

        return lAndExp;
    }

    template <>
    auto Parser1::_try<LOrExp>() -> u_ptr<LOrExp> {
        auto lOrExp = std::make_unique<LOrExp>(peekToken());

        // !!! NO ASSERT !!!
        lOrExp->lAndExps.push_back(_try<LAndExp>());
        while (peekToken().getType() == TK_OR) {
            skipToken();
            lOrExp->lAndExps.push_back(_try<LAndExp>());
        }

        return lOrExp;
    }

    template <>
    auto Parser1::_try<Cond>() -> u_ptr<Cond> {
        auto cond = std::make_unique<Cond>(peekToken());

        // !!! NO ASSERT !!!
        cond->lOrExp = _try<LOrExp>();

        return cond;
    }

    template <>
    auto Parser1::_try<Assignment>() -> u_ptr<Assignment> {
        auto assignment = std::make_unique<Assignment>(peekToken());

        assert(peekToken().getType() == TK_IDENFR);
        assignment->lVal = _try<LVal>();
        _matchCurToken(TK_ASSIGN);
        assignment->exp = _try<Exp>();

        return assignment;
    }

    template <>
    auto Parser1::_try<ForStmt>() -> u_ptr<ForStmt> {
        auto forStmt = std::make_unique<ForStmt>(peekToken());

        assert(peekToken().getType() == TK_FORTK);
        _matchCurToken(TK_FORTK);
        _matchCurToken(TK_LPARENT);

        if (peekToken().getType() == TK_IDENFR) {
            forStmt->init = _try<Assignment>();
        }
        _matchCurToken(TK_COMMA);

        forStmt->cond = _try<Cond>();
        _matchCurToken(TK_COMMA);

        if (peekToken().getType() == TK_IDENFR) {
            forStmt->update = _try<Assignment>();
        }
        _matchCurToken(TK_RPARENT);

        forStmt->stmt = _try<Stmt>();

        return forStmt;
    }

    template <>
    auto Parser1::_try<AssignStmt>() -> u_ptr<AssignStmt> {
        auto assignStmt = std::make_unique<AssignStmt>(peekToken());

        assert(peekToken().getType() == TK_IDENFR);
        assignStmt->assignment = _try<Assignment>();
        _matchCurToken(TK_SEMICN);

        return assignStmt;
    }

    template <>
    auto Parser1::_try<IfStmt>() -> u_ptr<IfStmt> {
        auto ifStmt = std::make_unique<IfStmt>(peekToken());
        // TODO
        return ifStmt;
    }

    template <>
    auto Parser1::_try<BreakStmt>() -> u_ptr<BreakStmt> {
        auto breakStmt = std::make_unique<BreakStmt>(peekToken());

        assert(peekToken().getType() == TK_BREAKTK);
        skipToken();
        _matchCurToken(TK_SEMICN);

        return breakStmt;
    }

    template <>
    auto Parser1::_try<ContinueStmt>() -> u_ptr<ContinueStmt> {
        auto continueStmt = std::make_unique<ContinueStmt>(peekToken());

        assert(peekToken().getType() == TK_CONTINUETK);
        skipToken();
        _matchCurToken(TK_SEMICN);

        return continueStmt;
    }

    template <>
    auto Parser1::_try<ReturnStmt>() -> u_ptr<ReturnStmt> {
        auto returnStmt = std::make_unique<ReturnStmt>(peekToken());

        assert(peekToken().getType() == TK_RETURNTK);
        skipToken();
        _matchCurToken(TK_SEMICN);

        return returnStmt;
    }

    template <>
    auto Parser1::_try<GetintStmt>() -> u_ptr<GetintStmt> {
        auto getintStmt = std::make_unique<GetintStmt>(peekToken());
        // TODO
        return getintStmt;
    }

    template <>
    auto Parser1::_try<GetcharStmt>() -> u_ptr<GetcharStmt> {
        auto getcharStmt = std::make_unique<GetcharStmt>(peekToken());
        // TODO
        return getcharStmt;
    }

    template <>
    auto Parser1::_try<PrintfStmt>() -> u_ptr<PrintfStmt> {
        auto printfStmt = std::make_unique<PrintfStmt>(peekToken());
        // TODO
        return printfStmt;
    }

    template <>
    auto Parser1::_try<Stmt>() -> u_ptr<Stmt> {
        auto stmt = std::make_unique<Stmt>(peekToken());
        // TODO
        return stmt;
    }

    template <>
    auto Parser1::_try<BlockItem>() -> u_ptr<BlockItem> {
        auto blockItem = std::make_unique<BlockItem>(peekToken());
        // TODO
        return blockItem;
    }

    template <>
    auto Parser1::_try<Block>() -> u_ptr<Block> {
        auto block = std::make_unique<Block>(peekToken());
        // TODO
        return block;
    }

    template <>
    auto Parser1::_try<FuncDef>() -> u_ptr<FuncDef> {
        auto funcDef = std::make_unique<FuncDef>(peekToken());
        // TODO
        return funcDef;
    }

    template <>
    auto Parser1::_try<Decl>() -> u_ptr<Decl> {
        auto decl = std::make_unique<Decl>(peekToken());
        // TODO
        return decl;
    }

    template <>
    auto Parser1::_try<MainFuncDef>() -> u_ptr<MainFuncDef> {
        auto mainFuncDef = std::make_unique<MainFuncDef>(peekToken());
        // TODO
        return mainFuncDef;
    }

    template <>
    auto Parser1::_try<CompUnit>() -> u_ptr<CompUnit> {
        auto compUnit = std::make_unique<CompUnit>(peekToken());
        // TODO
        return compUnit;
    }

    bool Parser1::_matchCurToken(TokenType expectType) {
        return _match(peekToken(), expectType);
    }

    bool Parser1::_match(Token&& t, TokenType expectType) {
        if (t.getType() != expectType) {
            // TODO: report error
            return false;
        }
        skipToken();
        return true;
    }

};