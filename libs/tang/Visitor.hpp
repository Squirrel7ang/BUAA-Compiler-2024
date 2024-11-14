//
// Created by tang on 10/19/24.
//

#ifndef VISITOR_HPP
#define VISITOR_HPP
#include <iostream>

#include "Ast.hpp"
#include "LoopStack.hpp"
#include "Symbol.hpp"
#include "ErrorReporter.hpp"
#include "IR/Common.hpp"
#include "IR/Module.hpp"
#include "IR/BasicBlock.hpp"

namespace tang {
    class Visitor {
        // tang related member
        SymbolTable _symbolTable;
        LoopStack _loopStack;
        u_ptr<CompUnit> _compUnit;
        ErrorReporter& _reporter;
        s_ptr<FuncSymbolType> _curFuncType; // TODO

        // LLVM related member
        llvm::ModulePtr _modulePtr;
        llvm::FunctionPtr _curFunction;
        llvm::BasicBlockPtr _curBlock;
    public:
        explicit Visitor(u_ptr<CompUnit>& compUnit, std::ostream& out,
                         ErrorReporter& reporter)
            : _compUnit(std::move(compUnit)), _loopStack(reporter),
              _symbolTable(out, reporter), _reporter(reporter) {
            _modulePtr = std::make_shared<llvm::Module>();
        }
        void visit();

    private:
        int evaluate(u_ptr<PrimaryExp> &node);
        int evaluate(u_ptr<UnaryExp> &node);
        int evaluate(u_ptr<MulExp> &node);
        int evaluate(u_ptr<AddExp> &node);
        int evaluate(u_ptr<Exp> &node);
        int evaluate(u_ptr<LVal> &node);
        int evaluate(u_ptr<Number> &node);
        int evaluate(u_ptr<Character> &node);
        int evaluate(u_ptr<ConstExp> &node);

        void _visitConstExp(const u_ptr<ConstExp>& node);
        void _visitInitVal(const u_ptr<InitVal>& node);
        void _visitConstInitVal(const u_ptr<ConstInitVal>& node);
        void _visitVarDef(const u_ptr<VarDef> &node, bool isInt, bool isChar);
        void _visitVarDecl(const u_ptr<VarDecl>& node);
        void _visitConstDef(const u_ptr<ConstDef> &constdef, bool isInt, bool isChar);
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

        void defineGlobalVariable(Symbol &s);
        void defineLocalVariable(u_ptr<VarDef>&, Symbol &s);
        llvm::ValuePtr genExpIR(u_ptr<Exp> &node);
        llvm::ValuePtr genAddExpIR(u_ptr<AddExp> &node);
        llvm::ValuePtr genMulExpIR(u_ptr<MulExp> &node);
        llvm::ValuePtr genUnaryExpIR(u_ptr<UnaryExp> &node);
        llvm::ValuePtr genPrimaryExp(u_ptr<PrimaryExp> &node);
        llvm::CallInstPtr genFuncCallIR(u_ptr<FuncCall> &node);
        llvm::ConstantDataPtr genNumberIR(u_ptr<Number> &node);
        llvm::ConstantDataPtr genCharacterIR(u_ptr<Character> &node);
        llvm::LoadInstPtr genLValIR(u_ptr<LVal> &node);
        void initLocalVariable(Symbol &s, llvm::ValuePtr ptr);
        void InitLocalVariable(Symbol &s);
        void initLocalArray(Symbol &s);
        void assignLocalVariable(Symbol &s);
        void loadLocalVariable(Symbol &s);
        void loadLocalArray(Symbol &s);

    private:
        bool isGlobal() { return _symbolTable.isGlobal(); }

    };
} // namespace tang

#endif //VISITOR_HPP
