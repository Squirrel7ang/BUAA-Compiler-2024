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

    void Visitor::_visitVarDecl(u_ptr<VarDecl>& node) {
        bool isConst = false;
        if (node->bType->isInt) {
            for (auto& vardef: node->varDefs) {
                if (vardef->is_array()) {
                    auto st = IntArrayType(isConst, vardef->constExp->evaluate());
                    Symbol s(st, vardef->ident->str);
                    _symbolTable.addSymbol(s);
                }
                else {
                    auto st = IntSymbolType(isConst);
                    Symbol s(st, vardef->ident->str);
                    _symbolTable.addSymbol(s);
                }
            }
        }
        else if (node->bType->isChar) {
            for (auto& vardef: node->varDefs) {
                if (vardef->is_array()) {
                    auto st = CharArrayType(isConst, vardef->constExp->evaluate());
                    Symbol s(st, vardef->ident->str);
                    _symbolTable.addSymbol(s);
                }
                else {
                    auto st = CharSymbolType(isConst);
                    Symbol s(st, vardef->ident->str);
                    _symbolTable.addSymbol(s);
                }
            }
        }
    }

    void Visitor::_visitConstDecl(u_ptr<ConstDecl>& node) {
        bool isConst = true;
        if (node->bType->isInt) {
            for (auto& constdef: node->constDefs) {
                if (constdef->is_array()) {
                    auto st = IntArrayType(isConst, constdef->constExp->evaluate());
                    Symbol s(st, constdef->ident->str);
                    _symbolTable.addSymbol(s);
                }
                else {
                    auto st = IntSymbolType(isConst);
                    Symbol s(st, constdef->ident->str);
                    _symbolTable.addSymbol(s);
                }
            }
        }
        else if (node->bType->isChar) {
            for (auto& constdef: node->constDefs) {
                if (constdef->is_array()) {
                    auto st = CharArrayType(isConst, constdef->constExp->evaluate());
                    Symbol s(st, constdef->ident->str);
                    _symbolTable.addSymbol(s);
                }
                else {
                    auto st = CharSymbolType(isConst);
                    Symbol s(st, constdef->ident->str);
                    _symbolTable.addSymbol(s);
                }
            }
        }
    }

    void Visitor::_visitDecl(u_ptr<Decl>& node) {
        // TODO
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

    void Visitor::_visitPrintfStmt(u_ptr<PrintfStmt>& node) {
        // TODO
    } 

    void Visitor::_visitGetcharStmt(u_ptr<GetcharStmt>& node) {
        // TODO
    } 

    void Visitor::_visitGetintStmt(u_ptr<GetintStmt>& node) {
        // TODO
    } 

    void Visitor::_visitReturnStmt(u_ptr<ReturnStmt>& node) {
        // TODO
    } 

    void Visitor::_visitContinueStmt(u_ptr<ContinueStmt>& node) {
        // TODO
    } 

    void Visitor::_visitBreakStmt(u_ptr<BreakStmt>& node) {
        // TODO
    } 

    void Visitor::_visitForStmt(u_ptr<ForStmt>& node) {
        // TODO
    } 

    void Visitor::_visitIfStmt(u_ptr<IfStmt>& node) {
        // TODO
    } 

    void Visitor::_visitExp(u_ptr<Exp>& node) {
        // TODO
    } 

    void Visitor::_visitAssignStmt(u_ptr<AssignStmt>& node) {
        // TODO
    } 

    void Visitor::_visitStmt(u_ptr<Stmt>& node) {
        std::visit([&](auto && arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, u_ptr<AssignStmt>>)
                _visitAssignStmt(arg);
            else if constexpr (std::is_same_v<T, u_ptr<Exp>>)
                _visitExp(arg);
            else if constexpr (std::is_same_v<T, u_ptr<Block>>)
                _visitBlock(arg);
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

    void Visitor::_visitBlock(u_ptr<Block>& node) {
        _symbolTable.enterScope();
        for (auto& blockItem: node->blockItems) {
            // use _visitDecl and _visitStmt;
            std::visit([&](auto && arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, u_ptr<Decl>>)
                    _visitDecl(arg);
                else if constexpr (std::is_same_v<T, u_ptr<Stmt>>)
                    _visitStmt(arg);
                else 
                    assert(0);
            }, *(blockItem->blockItem));
        }
        _symbolTable.exitScope();
    }

    void Visitor::_visitFuncDef(u_ptr<FuncDef>& node) {
        // get Function return Type and Identifier.
        SymbolType rt;
        vector<Symbol> fparams;
        if (node->funcType->isInt) {
            rt = IntSymbolType(false);
        }
        else if (node->funcType->isChar) {
            rt = CharSymbolType(false);
        }
        else if (node->funcType->isVoid) {
            rt = VoidSymbolType();
        }
        else {
            assert(0);
        }
        auto ft = FunctionType(rt);
        // get Function FParams Type and add to rt;
        for (auto& fparam: node->funcFParams->funcFParams) {
            SymbolType st;
            if (fparam->isArray) {
                if (fparam->bType->isInt) {
                    st = IntArrayType(false, 0);
                }
                else if (fparam->bType->isChar) {
                    st = CharArrayType(false, 0);
                }
                else {
                    assert(0);
                }
            }
            else {
                if (fparam->bType->isInt) {
                    st = IntSymbolType(false);
                }
                else if (fparam->bType->isChar) {
                    st = IntSymbolType(false);
                }
                else {
                    assert(0);
                }
            }
            ft.addArgType(st);
            Symbol s = Symbol(st, fparam->ident->str);
            fparams.push_back(s);
        }

        // create Symbol for function
        Symbol s = Symbol(ft, node->ident->str);
        _symbolTable.addSymbol(s);

        // afterwards, add FParams into symbolTable of a new scope
        _symbolTable.enterScope();
        for (auto s: fparams) {
            _symbolTable.addSymbol(s);
        }

        // then visit inside;
        _visitBlock(node->block);
        _symbolTable.exitScope();
    }

    void Visitor::_visitMainFuncDef(u_ptr<MainFuncDef>& node) {
        _symbolTable.enterScope();

        _visitBlock(node->block);
        _symbolTable.exitScope();
    }

    void Visitor::_visitCompUnit(u_ptr<CompUnit>& node) {
        for (auto& decl: node->decls) {
            _visitDecl(decl);
        }

        for (auto& funcDef: node->funcDefs) {
            _visitFuncDef(funcDef);
        }

        _visitMainFuncDef(node->mainFuncDef);
    }

    void Visitor::visit() {
        _symbolTable.enterScope();
        _visitCompUnit(_compUnit);
    }






} // namespace tang