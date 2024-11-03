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
    }

    void Visitor::_visitGetcharStmt(const u_ptr<GetcharStmt>& node) {
        // TODO
    } 

    void Visitor::_visitGetintStmt(const u_ptr<GetintStmt>& node) {
        // TODO
    } 

    void Visitor::_visitReturnStmt(const u_ptr<ReturnStmt>& node) {
        if (_curFuncType->toRawType() == VOID_FUNC_ST && node->exp != nullptr) {
            _reporter.report(node->returnToken.getLin(), 'f');
        }
    }

    void Visitor::_visitContinueStmt(const u_ptr<ContinueStmt>& node) {
        _loopStack.checkBreakContinue(node->getLin());
    } 

    void Visitor::_visitBreakStmt(const u_ptr<BreakStmt>& node) {
        _loopStack.checkBreakContinue(node->getLin());
    }

    void Visitor::_visitForStmt(const u_ptr<ForStmt>& node) {
        _loopStack.pushLoop();

        _symbolTable.enterScope();
        _visitStmt(node->stmt);
        _symbolTable.exitScope();

        _loopStack.popLoop();
    } 

    void Visitor::_visitIfStmt(const u_ptr<IfStmt>& node) {
        _loopStack.pushLoop();

        _symbolTable.enterScope();
        _visitStmt(node->ifStmt);
        _symbolTable.exitScope();

        if (node->elseStmt != nullptr) {
            _symbolTable.enterScope();
            _visitStmt(node->elseStmt);
            _symbolTable.exitScope();
        }

        _loopStack.popLoop();
    }

    void Visitor::_visitFuncCall(const u_ptr<FuncCall>& node, s_ptr<SymbolType> & type) {
        Symbol s;
        bool found;
        found = _symbolTable.findSymbolGlobal(s, node->ident->str);
        if (!found) {
            // _reporter.report(node->ident->getLin(), 'c');
            assert(0);
        }
        s_ptr<FuncSymbolType> symbolTypePtr = std::dynamic_pointer_cast<FuncSymbolType>(s.getType());
        assert(symbolTypePtr != nullptr);
        type = symbolTypePtr->_returnType;
        auto& rparamExps = node->funcRParams->exps;
        auto args = symbolTypePtr->_argType;
        if (rparamExps.size() != args.size()) {
            _reporter.report(node->ident->getLin(), 'd');
            return; // TODO
        }
        for (int i = 0; i < rparamExps.size(); i++) {
            auto& exp = rparamExps.at(i);
            s_ptr<SymbolType> argType;
            _visitExp(exp, argType);
            // TODO
            // if (exp->getType().toRawType() != args.at(i).toRawType()) {
            //     _reporter.report(node->ident->getLin(), 'e');
            // }
        }

    }

    void Visitor::_visitPrimaryExp(const u_ptr<PrimaryExp>& node, s_ptr<SymbolType>& type) {
        std::visit([&](auto && arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, u_ptr<Exp>>)
                _visitExp(arg, type);
            else if constexpr (std::is_same_v<T, u_ptr<LVal>>) {
                Symbol s;
                bool found = _symbolTable.findSymbolGlobal(s, arg->ident->str);
                if (!found) {
                    _reporter.report(arg->ident->getLin(), 'c');
                }
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

    void Visitor::_visitAssignStmt(const u_ptr<AssignStmt>& node) {
        // TODO
        Symbol s;
        bool found = _symbolTable.findSymbolGlobal(s, node->lVal->ident->str);
        if (!found) {
            _reporter.report(node->lVal->ident->getLin(), 'c');
        }
        if (s.getType()->isConst()) {
            _reporter.report(node->lVal->getLin(), 'h');
        }
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
                _symbolTable.enterScope();
                _visitBlock(arg);
                _symbolTable.exitScope();
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
            else 
                assert(0);
        }, *(node->stmt));
    }

    void Visitor::_visitBlock(const u_ptr<Block>& node) {
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

        // create Symbol for function
        _curFuncType = ft;
        Symbol s = Symbol(ft, node->ident->str);
        _symbolTable.addSymbol(node->ident->getLin(), s);

        // afterward, add FParams into symbolTable of a new scope
        _symbolTable.enterScope();
        for (auto s: fparams) {
            _symbolTable.addSymbol(node->ident->getLin(), s);
        }

        // then visit inside;
        _visitBlock(node->block);

        // check if there is return in the inside;
        auto& blockItem = node->block->blockItems.back()->blockItem;
        if (_curFuncType->toRawType() == VOID_FUNC_ST) {
            _symbolTable.exitScope();
            return;
        }
        if (!std::holds_alternative<u_ptr<Stmt>>(*blockItem)) {
            _reporter.report(node->block->rBrace.getLin(), 'g');
            _symbolTable.exitScope();
            return;
        }
        auto& stmt = std::get<u_ptr<Stmt>>(*blockItem)->stmt;
        if (!std::holds_alternative<u_ptr<ReturnStmt>>(*stmt)) {
            _reporter.report(node->block->rBrace.getLin(), 'g');
            _symbolTable.exitScope();
        }
    }

    void Visitor::_visitMainFuncDef(const u_ptr<MainFuncDef>& node) {
        auto rt = std::make_shared<IntSymbolType>();
        auto ft = std::make_shared<FuncSymbolType>(rt);
        _curFuncType = ft;
        _symbolTable.enterScope();

        _visitBlock(node->block);
        _symbolTable.exitScope();
    }

    void Visitor::_visitCompUnit(const u_ptr<CompUnit>& node) {
        _symbolTable.enterScope();
        for (auto& decl: node->decls) {
            _visitDecl(decl);
        }

        for (auto& funcDef: node->funcDefs) {
            _visitFuncDef(funcDef);
        }

        _visitMainFuncDef(node->mainFuncDef);
        _symbolTable.exitScope();
    }

    void Visitor::visit() {
        _visitCompUnit(_compUnit);
        _symbolTable.print();
    }

} // namespace tang