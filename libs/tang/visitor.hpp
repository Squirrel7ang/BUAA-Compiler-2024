//
// Created by tang on 10/19/24.
//

#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "ast.hpp"
#include "codeGen.hpp"
#include "symbol.hpp"
#include <iostream>

namespace tang {
    using namespace llvm;
    class Visitor {
        // CodeGen _codeGen;
        SymbolTable _symbolTable;
        u_ptr<CompUnit> _compUnit;
    public:
        explicit Visitor(u_ptr<CompUnit>& compUnit, std::ostream& out)
            : _compUnit(std::move(compUnit)), _symbolTable(out) { }
        void visit();
    private:
        void _visitVarDecl(const VarDecl& node);
        void _visitConstDecl(const ConstDecl& node);
        void _visitDecl(const Decl& node);
        void _visitFuncDef(const FuncDef& node);
        void _visitMainFuncDef(const MainFuncDef& node);
        void _visitCompUnit(const CompUnit& node);
    };
} // namespace tang

#endif //VISITOR_HPP
