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
        LoopStack _loopStack;
        u_ptr<CompUnit> _compUnit;
        ErrorReporter& _reporter;
        s_ptr<FuncSymbolType> _curFuncType; // TODO
    public:
        explicit Visitor(u_ptr<CompUnit>& compUnit, std::ostream& out,
                         ErrorReporter& reporter)
            : _compUnit(std::move(compUnit)), _loopStack(reporter),
              _symbolTable(out, reporter), _reporter(reporter) { }
        void visit();
    private:
        void _visitVarDecl(const u_ptr<VarDecl>& node);
        void _visitConstDecl(const u_ptr<ConstDecl>& node);
        void _visitDecl(const u_ptr<Decl>& node);
        void _visitPrintfStmt(const u_ptr<PrintfStmt>& node);
        void _visitGetcharStmt(const u_ptr<GetcharStmt>& node);
        void _visitGetintStmt(const u_ptr<GetintStmt>& node);
        void _visitReturnStmt(const u_ptr<ReturnStmt>& node);
        void _visitContinueStmt(const u_ptr<ContinueStmt>& node);
        void _visitBreakStmt(const u_ptr<BreakStmt>& node);
        void _visitLVal(const u_ptr<LVal>& node, bool unAssignable);
        void _visitLVal(const u_ptr<LVal>& node, s_ptr<SymbolType>& type, bool constIsUnassignable);
        void _visitAssignment(const u_ptr<Assignment>& node);
        void _visitForStmt(const u_ptr<ForStmt>& node);
        void _visitIfStmt(const u_ptr<IfStmt>& node);
        void _visitFuncCall(const u_ptr<FuncCall> &node, s_ptr<SymbolType>& type);
        void _visitPrimaryExp(const u_ptr<PrimaryExp> &node, s_ptr<SymbolType>& type);
        void _visitUnaryExp(const u_ptr<UnaryExp> &node, s_ptr<SymbolType>& type);
        void _visitMulExp(const u_ptr<MulExp> &node, s_ptr<SymbolType>& type);
        void _visitAddExp(const u_ptr<AddExp> &node, s_ptr<SymbolType>& type);
        void _visitBlock(const u_ptr<Block>& node, bool newScope, unsigned int scopeId);
        void _visitExp(const u_ptr<Exp>& node, s_ptr<SymbolType>& type);
        void _visitRelExp(const u_ptr<RelExp>& node, s_ptr<SymbolType>& type);
        void _visitEqExp(const u_ptr<EqExp>& node, s_ptr<SymbolType>& type);
        void _visitLAndExp(const u_ptr<LAndExp>& node, s_ptr<SymbolType>& type);
        void _visitLOrExp(const u_ptr<LOrExp>& node, s_ptr<SymbolType>& type);
        void _visitCond(const u_ptr<Cond>& node, s_ptr<SymbolType>& type);
        void _visitAssignStmt(const u_ptr<AssignStmt>& node);
        void _visitStmt(const u_ptr<Stmt>& node);
        void _visitFuncDef(const u_ptr<FuncDef>& node);
        void _visitMainFuncDef(const u_ptr<MainFuncDef>& node);
        void _visitCompUnit(const u_ptr<CompUnit>& node);
    };
} // namespace tang

#endif //VISITOR_HPP
