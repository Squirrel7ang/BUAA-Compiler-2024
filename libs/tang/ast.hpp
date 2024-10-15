//
// Created by tang on 10/13/24.
//

#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <variant>

namespace tang {
    class Exp;
    class Stmt;
    class UnaryOp;
    class FuncRParams;
    class CharConst;
    class IntConst;
    class LOrExp;
    class AddExp;
    class BlockItem;
    class FuncFParam;
    class Block;
    class FuncFParams;
    class FuncType;
    class InitVal;
    class VarDef;
    class StringConst;
    class ConstDef;
    class BType;
    class VarDecl;
    class ConstDecl;
    class ConstExp;
    class Ident;
    class MainFuncDef;
    class FuncDef;
    class Number;
    class Character;

    template <typename T>
    using u_ptr = std::unique_ptr<T>;

    template <typename T>
    using vector = std::vector<T>;

    template <typename ...Ts>
    using variant = std::variant<Ts...>;

    class Node {
        unsigned int _lin;
        unsigned int _col;
    public:
        [[nodiscard]] unsigned int getLin() const { return _lin; }
        [[nodiscard]] unsigned int getCol() const { return _col; }
        void setLin(const unsigned int lin) { _lin = lin; }
        void setCol(const unsigned int col) { _col = col; }

        friend std::ostream& operator << (std::ostream& os, const Node& node);
    };

    using DeclVariant = variant<ConstDecl, VarDecl>;
    class Decl: public Node {
    public:
        u_ptr<DeclVariant> decl;
    };

    class CompUnit: public Node {
    public:
        vector<u_ptr<Decl>> decls;
        vector<u_ptr<FuncDef>> funcDefs;
        u_ptr<MainFuncDef> mainFuncDef;

    };

    class ConstDecl: public Node {
    public:
        u_ptr<BType> bType;
        vector<u_ptr<ConstDef>> constDefs;
    };

    class ConstInitVal: public Node {
        vector<u_ptr<ConstExp>> constExps;
    public:
        u_ptr<StringConst> stringConst;
        void addConstExp(u_ptr<ConstExp>& constExp) {
            u_ptr<ConstExp> _constExp = std::move(constExp);
            constExps.push_back(std::move(_constExp));
        }
    };

    class BType: public Node {
    public:
        bool isInt;
        bool isChar;
        void setInt() { isInt = true, isChar = false; }
        void setChar() { isChar = true, isInt = false; }
    };

    class ConstDef: public Node {
    public:
        u_ptr<Ident> ident;
        u_ptr<ConstExp> constExp;
        u_ptr<ConstInitVal> constInitVal;
    };

    class VarDecl: public Node {
    public:
        u_ptr<BType> bType;
        vector<u_ptr<VarDef>> varDefs;

    };

    class VarDef: public Node {
    public:
        u_ptr<Ident> ident;
        u_ptr<ConstExp> constExp;
        u_ptr<InitVal> initVal;
    };

    class InitVal: public Node {
        vector<u_ptr<Exp>> exps;
    public:
        u_ptr<StringConst> stringConst;
        void addExp(u_ptr<Exp>& exp) {
            exps.push_back(std::move(exp));
            stringConst = nullptr;
        }
    };

    class FuncDef: public Node {
    public:
        u_ptr<FuncType> funcType;
        u_ptr<Ident> ident;
        u_ptr<FuncFParams> funcFParams;
        u_ptr<Block> block;
    };

    class MainFuncDef: public Node {
    public:
        u_ptr<Block> block;
    };

    class FuncType: public Node {
    public:
        bool isVoid;
        bool isInt;
        bool isChar;
        void setVoid() { isVoid = true; isInt = isChar = false; }
        void setInt() { isInt = true; isChar = isVoid = false; }
        void setChar() { isChar = true; isInt = isVoid = false; }
    };

    class FuncFParams: public Node {
    public:
        vector<u_ptr<FuncFParam>> funcFParams;
    };

    class FuncFParam: public Node {
    public:
        u_ptr<BType> bType;
        u_ptr<Ident> ident;
        bool isArray;
    };

    class Block: public Node {
    public:
        u_ptr<BlockItem> blockItem;

    };

    class BlockItem: public Node {
    public:
        u_ptr<variant<Decl, Stmt>> blockItem;
    };

    class Assignment: public Node { // this is the ForStmt in the ducoment

    };

    class AssignStmt: public Node {

    };

    class IfStmt: public Node {

    };

    class ForStmt: public Node { // this is NOT the ForStmt in the document

    };

    class BreakStmt: public Node {

    };

    class ContinueStmt: public Node {

    };

    class ReturnStmt: public Node {

    };

    class GetintStmt: public Node {

    };

    class GetcharStmt: public Node {

    };

    class PrintfStmt: public Node {

    };

    class Stmt: public Node {
        // TODO: variant of the above stmt;

    };

    class Exp: public Node {
    public:
        u_ptr<AddExp> addExp;
    };

    class Cond: public Node {
    public:
        u_ptr<LOrExp> lOrExp;
    };

    class LVal: public Node {
    public:
        u_ptr<Ident> ident;
        u_ptr<Exp> exp;
    };

    using PrimaryExpVariant = variant<Exp, LVal, Number, Character>;
    class PrimaryExp: public Node {
    public:
        u_ptr<PrimaryExpVariant> primaryExp;
    };

    class Number: public Node {
    public:
        u_ptr<IntConst> intConst;
    };

    class Character: public Node {
    public:
        u_ptr<CharConst> charConst;
    };

    class FuncCall: public Node {
    public:
        u_ptr<Ident> ident;
        u_ptr<FuncRParams> funcRParams;
    };

    using UnaryExpVariant = variant<PrimaryExp, FuncCall>;
    class UnaryExp: public Node {
    public:
        vector<u_ptr<UnaryOp>> unaryOps;
        u_ptr<UnaryExpVariant> unaryExp;

    };

    class UnaryOp: public Node {
    public:
        bool isPlus;
        bool isMinus;
        bool isExc;
        void setPlus() { isPlus = true; isMinus = isExc = false; }
        void setMinus() { isMinus = true; isPlus = isExc = false; }
        void setExc() { isExc = true; isMinus = isPlus = false; }
    };

    class FuncRParams: public Node {
    public:
        vector<u_ptr<Exp>> exps;
    };

    class MulExp: public Node {
    public:
        vector<u_ptr<UnaryExp>> unaryExps;
        vector<Token> ops;
    };

    class AddExp: public Node {
    public:
        vector<u_ptr<MulExp>> mulExps;
        vector<Token> ops;
    };

    class RelExp: public Node {
    public:
        vector<u_ptr<AddExp>> addExps;
        vector<Token> ops;
    };

    class EqExp: public Node {
    public:
        vector<u_ptr<RelExp>> relExps;
        vector<Token> ops;
    };

    class LAndExp: public Node {
    public:
        vector<u_ptr<EqExp>> eqExps;
    };

    class LOrExp: public Node {
    public:
        vector<u_ptr<LAndExp>> lAndExps;
    };

    class ConstExp: public Node {
    public:
        u_ptr<AddExp> addExp;
    };

    class Ident: public Node {
    public:
        std::string str;

    };

    class IntConst: public Node {
    public:
        int val;
    };

    class CharConst: public Node {
    public:
        char ch;

    };

    class StringConst: public Node {
    public:
        u_ptr<std::string> str;

    };
} // namespace tang

#endif //AST_HPP
