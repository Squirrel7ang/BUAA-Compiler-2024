//
// Created by tang on 10/19/24.
//

#include "visitor.hpp"

namespace tang {
    void Visitor::_visitDecl(const Decl& node) {
        // TODO

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