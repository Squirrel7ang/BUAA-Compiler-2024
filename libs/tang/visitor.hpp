//
// Created by tang on 10/19/24.
//

#ifndef VISITOR_HPP
#define VISITOR_HPP
#include <iostream>

#include "ast.hpp"
#include "codeGen.hpp"
#include "loopStack.hpp"
#include "symbol.hpp"
#include "error.hpp"

namespace tang {
    using namespace llvm;
    class Visitor {
        // CodeGen _codeGen;
        SymbolTable _symbolTable;
        u_ptr<CompUnit> _compUnit;
    public:
        explicit Visitor(u_ptr<CompUnit>& compUnit, std::ostream& out, ErrorReporter& reporter)
            : _compUnit(std::move(compUnit)), _symbolTable(out, reporter) { }
        void visit();
    private:
        void _visitVarDecl(u_ptr<VarDecl>& node);
        void _visitConstDecl(u_ptr<ConstDecl>& node);
        void _visitDecl(u_ptr<Decl>& node);
        void Visitor::_visitPrintfStmt(u_ptr<PrintfStmt>& node);
        void Visitor::_visitGetcharStmt(u_ptr<GetcharStmt>& node);
        void Visitor::_visitGetintStmt(u_ptr<GetintStmt>& node);
        void Visitor::_visitReturnStmt(u_ptr<ReturnStmt>& node);
        void Visitor::_visitContinueStmt(u_ptr<ContinueStmt>& node);
        void Visitor::_visitBreakStmt(u_ptr<BreakStmt>& node);
        void Visitor::_visitForStmt(u_ptr<ForStmt>& node);
        void Visitor::_visitIfStmt(u_ptr<IfStmt>& node);
        void Visitor::_visitBlock(u_ptr<Block>& node);
        void Visitor::_visitExp(u_ptr<Exp>& node);
        void Visitor::_visitAssignStmt(u_ptr<AssignStmt>& node);
        void _visitStmt(u_ptr<Stmt>& node);
        void _visitBlock(u_ptr<Block>& node);
        void _visitFuncDef(u_ptr<FuncDef>& node);
        void _visitMainFuncDef(u_ptr<MainFuncDef>& node);
        void _visitCompUnit(u_ptr<CompUnit>& node);
    };
} // namespace tang

#endif //VISITOR_HPP
