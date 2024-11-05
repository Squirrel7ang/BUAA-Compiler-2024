//
// Created by tang on 10/19/24.
//

#include "visitor.hpp"
#include "ast.hpp"
#include <cassert>

namespace tang {
    // template<class... Ts>
    // struct overloaded : Ts... { using Ts::operator()...; };
    // template<class... Ts>
    // overloaded(Ts...) -> overloaded<Ts...>;

    void Visitor::_visitConstExp(const u_ptr<ConstExp>& node) {
        s_ptr<SymbolType> _;
        _visitAddExp(node->addExp, _);
    }

    void Visitor::_visitInitVal(const u_ptr<InitVal>& node) {
        s_ptr<SymbolType> _;
        for (auto& exp: node->exps) {
            _visitExp(exp, _);
        }
        if (node->stringConst != nullptr) {
            // TODO: visitStringConst
        }
    }

    void Visitor::_visitConstInitVal(const u_ptr<ConstInitVal>& node) {
        s_ptr<SymbolType> _;
        for (auto& constExp: node->constExps) {
            _visitConstExp(constExp);
        }
        if (node->stringConst != nullptr) {
            // TODO: visitStringConst
        }
    }

    void Visitor::_visitVarDecl(const u_ptr<VarDecl>& node) {
        constexpr bool isConst = false;
        if (node->bType->isInt) {
            for (auto& vardef: node->varDefs) {
                if (vardef->is_array()) {
                    auto st = std::make_shared<IntArrayType>(isConst, vardef->constExp->evaluate());
                    Symbol s(st, vardef->ident->str);
                    _symbolTable.addSymbol(vardef->ident->getLin(), s);
                }
                else {
                    auto st = std::make_shared<IntSymbolType>(isConst);
                    Symbol s(st, vardef->ident->str);
                    _symbolTable.addSymbol(vardef->ident->getLin(), s);
                }

                if (vardef->constExp != nullptr) {
                    _visitConstExp(vardef->constExp);
                }
                if (vardef->initVal != nullptr) {
                    _visitInitVal(vardef->initVal);
                }
            }
        }
        else if (node->bType->isChar) {
            for (auto& vardef: node->varDefs) {
                if (vardef->is_array()) {
                    auto st = std::make_shared<CharArrayType>(isConst, vardef->constExp->evaluate());
                    Symbol s(st, vardef->ident->str);
                    _symbolTable.addSymbol(vardef->ident->getLin(), s);
                }
                else {
                    auto st = std::make_shared<CharSymbolType>(isConst);
                    Symbol s(st, vardef->ident->str);
                    _symbolTable.addSymbol(vardef->ident->getLin(), s);
                }

                if (vardef->constExp != nullptr) {
                    _visitConstExp(vardef->constExp);
                }
                if (vardef->initVal != nullptr) {
                    _visitInitVal(vardef->initVal);
                }
            }
        }
    }

    void Visitor::_visitConstDecl(const u_ptr<ConstDecl>& node) {
        bool isConst = true;
        if (node->bType->isInt) {
            for (auto& constdef: node->constDefs) {
                if (constdef->is_array()) {
                    auto st = std::make_shared<IntArrayType>(isConst, constdef->constExp->evaluate());
                    Symbol s(st, constdef->ident->str);
                    _symbolTable.addSymbol(constdef->ident->getLin(), s);
                }
                else {
                    auto st = std::make_shared<IntSymbolType>(isConst);
                    Symbol s(st, constdef->ident->str);
                    _symbolTable.addSymbol(constdef->ident->getLin(), s);
                }

                if (constdef->constExp != nullptr) {
                    _visitConstExp(constdef->constExp);
                }
                if (constdef->constInitVal != nullptr) {
                    _visitConstInitVal(constdef->constInitVal);
                }
            }
        }
        else if (node->bType->isChar) {
            for (auto& constdef: node->constDefs) {
                if (constdef->is_array()) {
                    auto st = std::make_shared<CharArrayType>(isConst, constdef->constExp->evaluate());
                    Symbol s(st, constdef->ident->str);
                    _symbolTable.addSymbol(constdef->ident->getLin(), s);
                }
                else {
                    auto st = std::make_shared<CharSymbolType>(isConst);
                    Symbol s(st, constdef->ident->str);
                    _symbolTable.addSymbol(constdef->ident->getLin(), s);
                }

                if (constdef->constExp != nullptr) {
                    _visitConstExp(constdef->constExp);
                }
                if (constdef->constInitVal != nullptr) {
                    _visitConstInitVal(constdef->constInitVal);
                }
            }
        }
    }

    void Visitor::_visitDecl(const u_ptr<Decl>& node) {
        auto& decl = (node->decl);
        std::visit([&](auto && arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, u_ptr<VarDecl>>)
                _visitVarDecl(arg);
            else if constexpr (std::is_same_v<T, u_ptr<ConstDecl>>)
                _visitConstDecl(arg);
            else 
                assert(0);
        }, *decl);
    }

    void Visitor::_visitPrintfStmt(const u_ptr<PrintfStmt>& node) {
        unsigned int formatNum = node->stringConst->getFormatNum();
        if (formatNum != node->exps.size()) {
            _reporter.report(node->printfToken.getLin(), 'l');
        }
        for (auto& exp: node->exps) {
            s_ptr<SymbolType> _;
            _visitExp(exp, _);
        }
    }

    void Visitor::_visitGetcharStmt(const u_ptr<GetcharStmt>& node) {
        _visitLVal(node->lVal, true);
        // TODO
    }

    void Visitor::_visitGetintStmt(const u_ptr<GetintStmt>& node) {
        _visitLVal(node->lVal, true);
        // TODO
    } 

    void Visitor::_visitReturnStmt(const u_ptr<ReturnStmt>& node) {
        if (_curFuncType->toRawType() == VOID_FUNC_ST && node->exp != nullptr) {
            _reporter.report(node->returnToken.getLin(), 'f');
        }
        s_ptr<SymbolType> _;
        if (node->exp != nullptr) {
            _visitExp(node->exp, _);
        }
    }

    void Visitor::_visitContinueStmt(const u_ptr<ContinueStmt>& node) {
        _loopStack.checkBreakContinue(node->getLin());
    } 

    void Visitor::_visitBreakStmt(const u_ptr<BreakStmt>& node) {
        _loopStack.checkBreakContinue(node->getLin());
    }

    void Visitor::_visitLVal(const u_ptr<LVal>& node,
                             s_ptr<SymbolType>& type,
                             bool constIsUnassignable) {
        Symbol s;

        bool found = _symbolTable.findSymbolGlobal(s, node->ident->str);
        if (!found) {
            _reporter.report(node->ident->getLin(), 'c');
            type = nullptr;
            return;
        }
        else if (constIsUnassignable && !s.getType()->isFunc() && s.getType()->isConst()) { // TODO: when s is funcSymbol
            _reporter.report(node->getLin(), 'h');
        }

        if (s.getType()->isArray() && node->exp != nullptr) {
            // this is like a[0]
            if (s.getType()->toRawType() == INT_ARRAY_ST) {
                type = std::make_shared<IntSymbolType>(false);
            }
            else if (s.getType()->toRawType() == CONST_INT_ARRAY_ST) {
                type = std::make_shared<IntSymbolType>(true);
            }
            else if (s.getType()->toRawType() == CHAR_ARRAY_ST) {
                type = std::make_shared<CharSymbolType>(false);
            }
            else if (s.getType()->toRawType() == CONST_CHAR_ARRAY_ST) {
                type = std::make_shared<CharSymbolType>(true);
            }
        }
        else if (!s.getType()->isFunc()) {
            type = s.getType();
        }
        else {
            type = nullptr;
        }
    }

    void Visitor::_visitLVal(const u_ptr<LVal>& node, bool unAssignable) {
        Symbol s;

        bool found = _symbolTable.findSymbolGlobal(s, node->ident->str);
        if (!found) {
            _reporter.report(node->ident->getLin(), 'c');
        }
        else if (unAssignable && !s.getType()->isFunc() && s.getType()->isConst()) { // TODO: when s is funcSymbol
            _reporter.report(node->getLin(), 'h');
        }
    }

    void Visitor::_visitAssignment(const u_ptr<Assignment>& node) {
        Symbol s;

        _visitLVal(node->lVal, true);

        s_ptr<SymbolType> _;
        _visitExp(node->exp, _);
    }

    void Visitor::_visitForStmt(const u_ptr<ForStmt>& node) {
        _loopStack.pushLoop();
        s_ptr<SymbolType> _;

        if (node->init != nullptr) {
            _visitAssignment(node->init);
        }
        if (node->cond != nullptr) {
            _visitCond(node->cond, _);
        }

        if (node->update != nullptr) {
            _visitAssignment(node->update);
        }

        _visitStmt(node->stmt);

        _loopStack.popLoop();
    }

    void Visitor::_visitIfStmt(const u_ptr<IfStmt>& node) {
        s_ptr<SymbolType> _;
        if (node->cond != nullptr) {
            _visitCond(node->cond, _);
        }

        _visitStmt(node->ifStmt);

        if (node->elseStmt != nullptr) {
            _visitStmt(node->elseStmt);
        }
    }

    void Visitor::_visitFuncCall(const u_ptr<FuncCall>& node, s_ptr<SymbolType> & type) {
        Symbol s;
        bool found;
        found = _symbolTable.findSymbolGlobal(s, node->ident->str);
        if (!found) {
            _reporter.report(node->ident->getLin(), 'c');
            return; // TODO
        }
        s_ptr<FuncSymbolType> funcSymbolTypePtr = std::dynamic_pointer_cast<FuncSymbolType>(s.getType());
        assert(funcSymbolTypePtr != nullptr);
        type = funcSymbolTypePtr->_returnType;
        auto& rparamExps = node->funcRParams->exps;
        auto args = funcSymbolTypePtr->_argType;
        auto rparamSize = node->funcRParams == nullptr ? 0 : rparamExps.size();
        if (rparamSize != args.size()) {
            _reporter.report(node->ident->getLin(), 'd');
            // return; // TODO
        }
        for (int i = 0; i < rparamSize; i++) {
            auto& exp = rparamExps.at(i);
            s_ptr<SymbolType> rArgType;
            _visitExp(exp, rArgType);
            if (rArgType == nullptr) {
                continue; // TODO
            }
            if (i >= funcSymbolTypePtr->_argType.size()) {
                continue; // TODO
            }

            auto& fArgType = funcSymbolTypePtr->_argType.at(i);
            if (fArgType->isArray() != rArgType->isArray()) {
                _reporter.report(node->ident->getLin(), 'e');
                // break; // TODO
            }
            else if (fArgType->isArray() && rArgType->isArray()) {
                // both array but with different array type
                RawSymbolType fType = fArgType->toRawType();
                RawSymbolType pType = rArgType->toRawType();
                if ((fType == INT_ARRAY_ST || fType == CONST_INT_ARRAY_ST) &&
                    (pType == CHAR_ARRAY_ST || pType == CONST_CHAR_ARRAY_ST)) {
                    _reporter.report(node->ident->getLin(), 'e');
                    // break; // TODO
                }
                else if ((fType == CHAR_ARRAY_ST || fType == CONST_CHAR_ARRAY_ST) &&
                    (pType == INT_ARRAY_ST || pType == CONST_INT_ARRAY_ST)) {
                    _reporter.report(node->ident->getLin(), 'e');
                    // break; // TODO
                }
            }
        }
    }

    void Visitor::_visitPrimaryExp(const u_ptr<PrimaryExp>& node, s_ptr<SymbolType>& type) {
        std::visit([&](auto && arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, u_ptr<Exp>>)
                _visitExp(arg, type);
            else if constexpr (std::is_same_v<T, u_ptr<LVal>>) {
                _visitLVal(arg, type, false);
            }
            else if constexpr (std::is_same_v<T, u_ptr<Number>>) {
                type = std::make_shared<IntSymbolType>(false);
                // TODO
            }
            else if constexpr (std::is_same_v<T, u_ptr<Character>>) {
                type = std::make_shared<CharSymbolType>(false);
                // TODO
            }
            else
                assert(0);
        }, *(node->primaryExp));
    }

    void Visitor::_visitUnaryExp(const u_ptr<UnaryExp>& node, s_ptr<SymbolType>& type) {
        std::visit([&](auto && arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, u_ptr<PrimaryExp>>)
                _visitPrimaryExp(arg, type);
            else if constexpr (std::is_same_v<T, u_ptr<FuncCall>>)
                _visitFuncCall(arg, type);
            else
                assert(0);
        }, *(node->unaryExp));
        if (node->unaryOps.size() > 0) {
            type = std::make_shared<IntSymbolType>(false);
        }
    }

    void Visitor::_visitMulExp(const u_ptr<MulExp>& node, s_ptr<SymbolType>& type) {
        for (auto& unaryexp: node->unaryExps) {
            _visitUnaryExp(unaryexp, type);
        }
        if (node->unaryExps.size() > 1) {
            type = std::make_shared<IntSymbolType>(false);
        }
    }

    void Visitor::_visitAddExp(const u_ptr<AddExp>& node, s_ptr<SymbolType>& type) {
        for (auto& mulexp: node->mulExps) {
            _visitMulExp(mulexp, type);
        }
        if (node->mulExps.size() > 1) {
            type = std::make_shared<IntSymbolType>(false);
        }
    }

    void Visitor::_visitExp(const u_ptr<Exp>& node, s_ptr<SymbolType>& type) {
        _visitAddExp(node->addExp, type);
    }

    void Visitor::_visitRelExp(const u_ptr<RelExp>& node, s_ptr<SymbolType>& type) {
        for (auto& addExp: node->addExps) {
            _visitAddExp(addExp, type);
        }
        if (node->addExps.size() > 1) {
            type = std::make_shared<IntSymbolType>(false);
        }
    }

    void Visitor::_visitEqExp(const u_ptr<EqExp>& node, s_ptr<SymbolType>& type) {
        for (auto& relExp: node->relExps) {
            _visitRelExp(relExp, type);
        }
        if (node->relExps.size() > 1) {
            type = std::make_shared<IntSymbolType>(false);
        }
    }

    void Visitor::_visitLAndExp(const u_ptr<LAndExp>& node, s_ptr<SymbolType>& type) {
        for (auto& eqExp: node->eqExps) {
            _visitEqExp(eqExp, type);
        }
        if (node->eqExps.size() > 1) {
            type = std::make_shared<IntSymbolType>(false);
        }
    }

    void Visitor::_visitLOrExp(const u_ptr<LOrExp>& node, s_ptr<SymbolType>& type) {
        for (auto& lAndExp: node->lAndExps) {
            _visitLAndExp(lAndExp, type);
        }
        if (node->lAndExps.size() > 1) {
            type = std::make_shared<IntSymbolType>(false);
        }
    }

    void Visitor::_visitCond(const u_ptr<Cond>& node, s_ptr<SymbolType>& type) {
        _visitLOrExp(node->lOrExp, type);
    }

    void Visitor::_visitAssignStmt(const u_ptr<AssignStmt>& node) {
        // TODO
        s_ptr<SymbolType> _;
        _visitLVal(node->lVal, true);
        _visitExp(node->exp, _);
    }

    void Visitor::_visitStmt(const u_ptr<Stmt>& node) {
        s_ptr<SymbolType> p;
        std::visit([&](auto && arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, u_ptr<AssignStmt>>)
                _visitAssignStmt(arg);
            else if constexpr (std::is_same_v<T, u_ptr<Exp>>)
                _visitExp(arg, p);
            else if constexpr (std::is_same_v<T, u_ptr<Block>>) {
                _visitBlock(arg, true, 0);
            }
            else if constexpr (std::is_same_v<T, u_ptr<IfStmt>>)
                _visitIfStmt(arg);
            else if constexpr (std::is_same_v<T, u_ptr<ForStmt>>)
                _visitForStmt(arg);
            else if constexpr (std::is_same_v<T, u_ptr<BreakStmt>>)
                _visitBreakStmt(arg);
            else if constexpr (std::is_same_v<T, u_ptr<ContinueStmt>>)
                _visitContinueStmt(arg);
            else if constexpr (std::is_same_v<T, u_ptr<ReturnStmt>>)
                _visitReturnStmt(arg);
            else if constexpr (std::is_same_v<T, u_ptr<GetintStmt>>)
                _visitGetintStmt(arg);
            else if constexpr (std::is_same_v<T, u_ptr<GetcharStmt>>)
                _visitGetcharStmt(arg);
            else if constexpr (std::is_same_v<T, u_ptr<PrintfStmt>>)
                _visitPrintfStmt(arg);
            else if constexpr (std::is_same_v<T, u_ptr<EmptyStmt>>) { /* do nothing */ }
            else
                assert(0);
        }, *(node->stmt));
    }

    void Visitor::_visitBlock(const u_ptr<Block>& node, bool newScope, unsigned int scopeId) {
        if (newScope) {
            scopeId = _symbolTable.enterScope();
        }
        for (auto& blockItem: node->blockItems) {
            // use _visitDecl and _visitStmt;
            std::visit([&](auto && arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, u_ptr<Decl>>) {
                    _visitDecl(arg);
                }
                else if constexpr (std::is_same_v<T, u_ptr<Stmt>>) {
                    _visitStmt(arg);
                }
                else {
                    assert(0);
                }
            }, *(blockItem->blockItem));
        }
        _symbolTable.exitScope(scopeId);
    }

    void Visitor::_visitFuncDef(const u_ptr<FuncDef>& node) {
        // get Function return Type and Identifier.
        s_ptr<SymbolType> rt;
        vector<Symbol> fparams;
        if (node->funcType->isInt) {
            rt = std::make_shared<IntSymbolType>(false);
        }
        else if (node->funcType->isChar) {
            rt = std::make_shared<CharSymbolType>(false);
        }
        else if (node->funcType->isVoid) {
            rt = std::make_shared<VoidSymbolType>();
        }
        else {
            assert(0);
        }
        auto ft = std::make_shared<FuncSymbolType>(rt);
        // get Function FParams Type and add to rt;
        if (node->funcFParams  != nullptr) {
            for (auto& fparam: node->funcFParams->funcFParams) {
                s_ptr<SymbolType> st;
                const bool isConst = false;
                if (fparam->isArray) {
                    if (fparam->bType->isInt) {
                        st = std::make_shared<IntArrayType>(isConst, 0);
                    }
                    else if (fparam->bType->isChar) {
                        st = std::make_shared<CharArrayType>(isConst, 0);
                    }
                    else {
                        assert(0);
                    }
                }
                else {
                    if (fparam->bType->isInt) {
                        st = std::make_shared<IntSymbolType>(isConst);
                    }
                    else if (fparam->bType->isChar) {
                        st = std::make_shared<CharSymbolType>(isConst);
                    }
                    else {
                        assert(0);
                    }
                }
                ft->addArgType(st);
                Symbol s = Symbol(st, fparam->ident->str);
                fparams.push_back(s);
            }
        }

        // create Symbol for function
        _curFuncType = ft;
        Symbol s = Symbol(ft, node->ident->str);
        _symbolTable.addSymbol(node->ident->getLin(), s);

        // afterward, add FParams into symbolTable of a new scope
        auto scopeIndex = _symbolTable.enterScope();
        for (auto s: fparams) {
            _symbolTable.addSymbol(node->ident->getLin(), s);
        }

        // then visit inside;
        _visitBlock(node->block, false, scopeIndex);

        // check if there is return in the inside;
        if (_curFuncType->toRawType() == VOID_FUNC_ST) {
            return;
        }
        if (node->block->blockItems.size() == 0) {
            _reporter.report(node->block->rBrace.getLin(), 'g');
            return;
        }
        auto& blockItem = node->block->blockItems.back()->blockItem;
        if (!std::holds_alternative<u_ptr<Stmt>>(*blockItem)) {
            _reporter.report(node->block->rBrace.getLin(), 'g');
            return;
        }
        auto& stmt = std::get<u_ptr<Stmt>>(*blockItem)->stmt;
        if (!std::holds_alternative<u_ptr<ReturnStmt>>(*stmt)) {
            _reporter.report(node->block->rBrace.getLin(), 'g');
        }
    }

    void Visitor::_visitMainFuncDef(const u_ptr<MainFuncDef>& node) {
        auto rt = std::make_shared<IntSymbolType>();
        auto ft = std::make_shared<FuncSymbolType>(rt);
        _curFuncType = ft;
        _visitBlock(node->block, true, 0);

        // check if there is return in the inside;
        if (_curFuncType->toRawType() == VOID_FUNC_ST) {
            return;
        }
        if (node->block->blockItems.size() == 0) {
            _reporter.report(node->block->rBrace.getLin(), 'g');
            return;
        }
        auto& blockItem = node->block->blockItems.back()->blockItem;
        if (!std::holds_alternative<u_ptr<Stmt>>(*blockItem)) {
            _reporter.report(node->block->rBrace.getLin(), 'g');
            return;
        }
        auto& stmt = std::get<u_ptr<Stmt>>(*blockItem)->stmt;
        if (!std::holds_alternative<u_ptr<ReturnStmt>>(*stmt)) {
            _reporter.report(node->block->rBrace.getLin(), 'g');
        }
    }

    void Visitor::_visitCompUnit(const u_ptr<CompUnit>& node) {
        auto scopeIndex = _symbolTable.enterScope();
        for (auto& decl: node->decls) {
            _visitDecl(decl);
        }

        for (auto& funcDef: node->funcDefs) {
            _visitFuncDef(funcDef);
        }

        _visitMainFuncDef(node->mainFuncDef);
        _symbolTable.exitScope(scopeIndex);
    }

    void Visitor::visit() {
        _visitCompUnit(_compUnit);
        _symbolTable.print();
    }

} // namespace tang