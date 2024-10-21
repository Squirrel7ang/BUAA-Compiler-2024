//
// Created by tang on 10/19/24.
//

#include "visitor.hpp"
#include "ast.hpp"
#include <cassert>

namespace tang {

    void Visitor::_visitVarDecl(const VarDecl& node) {

    }

    void Visitor::_visitConstDecl(const ConstDecl& node) {

    }

    void Visitor::_visitDecl(const Decl& node) {
        // TODO
        DeclVariant decl = *(node.decl);
        std::visit(overloaded {
            [](u_ptr<VarDecl> arg) { _visitVarDecl(arg); },
            [](u_ptr<VarDecl> arg) { _visitConstDecl(arg); },
        }, decl);
    }

    void Visitor::_visitFuncDef(const FuncDef& node) {

    }

    void Visitor::_visitMainFuncDef(const MainFuncDef& node) {

    }

    void Visitor::_visitCompUnit(const CompUnit& node) {
        for (auto decl: node.decls) {
            _visitDecl(*decl);
        }

        for (auto funcDef: node.funcDefs) {
            _visitFuncDef(*funcDef);
        }

        _visitMainFuncDef(*node.mainFuncDef);
    }

    void Visitor::visit() {
        _visitCompUnit(*_compUnit);
    }






} // namespace tang