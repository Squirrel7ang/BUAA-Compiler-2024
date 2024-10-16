//
// Created by tang on 10/13/24.
//

#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <variant>

namespace tang {
    class Cond;
    class LVal;
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
        explicit Node(const Token& t) : _lin(t.getLin()), _col(t.getCol()) {}
        [[nodiscard]] unsigned int getLin() const { return _lin; }
        [[nodiscard]] unsigned int getCol() const { return _col; }
        void setLin(const unsigned int lin) { _lin = lin; }
        void setCol(const unsigned int col) { _col = col; }

        friend std::ostream& operator << (std::ostream& os, const Node& node);
    };

    using DeclVariant = variant<u_ptr<ConstDecl>, u_ptr<VarDecl>>;
    class Decl: public Node {
    public:
        explicit Decl(const Token& t) : Node(t) {}
        u_ptr<DeclVariant> decl;
    };

    class CompUnit: public Node {
    public:
        explicit CompUnit(const Token& t) : Node(t) {}
        vector<u_ptr<Decl>> decls;
        vector<u_ptr<FuncDef>> funcDefs;
        u_ptr<MainFuncDef> mainFuncDef;
    };

    class ConstDecl: public Node {
    public:
        explicit ConstDecl(const Token& t) : Node(t) {}
        u_ptr<BType> bType;
        vector<u_ptr<ConstDef>> constDefs;
    };

    class ConstInitVal: public Node {
        vector<u_ptr<ConstExp>> constExps;
            explicit ConstInitVal(const Token& t) : Node(t) {}
    public:
        u_ptr<StringConst> stringConst;
        void addConstExp(u_ptr<ConstExp>& constExp) {
            u_ptr<ConstExp> _constExp = std::move(constExp);
            constExps.push_back(std::move(_constExp));
        }
    };

    class BType: public Node {
    public:
        explicit BType(const Token& t) : Node(t) {}
        bool isInt;
        bool isChar;
        void setInt() { isInt = true, isChar = false; }
        void setChar() { isChar = true, isInt = false; }
    };

    class ConstDef: public Node {
    public:
        explicit ConstDef(const Token& t) : Node(t) {}
        u_ptr<Ident> ident;
        u_ptr<ConstExp> constExp;
        u_ptr<ConstInitVal> constInitVal;
    };

    class VarDecl: public Node {
    public:
        explicit VarDecl(const Token& t) : Node(t) {}
        u_ptr<BType> bType;
        vector<u_ptr<VarDef>> varDefs;
    };

    class VarDef: public Node {
    public:
        explicit VarDef(const Token& t) : Node(t) {}
        u_ptr<Ident> ident;
        u_ptr<ConstExp> constExp;
        u_ptr<InitVal> initVal;
    };

    class InitVal: public Node {
        vector<u_ptr<Exp>> exps;
            explicit InitVal(const Token& t) : Node(t) {}
    public:
        u_ptr<StringConst> stringConst;
        void addExp(u_ptr<Exp>& exp) {
            exps.push_back(std::move(exp));
            stringConst = nullptr;
        }
    };

    class FuncDef: public Node {
    public:
        explicit FuncDef(const Token& t) : Node(t) {}
        u_ptr<FuncType> funcType;
        u_ptr<Ident> ident;
        u_ptr<FuncFParams> funcFParams;
        u_ptr<Block> block;
    };

    class MainFuncDef: public Node {
    public:
        explicit MainFuncDef(const Token& t) : Node(t) {}
        u_ptr<Block> block;
    };

    class FuncType: public Node {
    public:
        explicit FuncType(const Token& t) : Node(t) {}
        bool isVoid;
        bool isInt;
        bool isChar;
        void setVoid() { isVoid = true; isInt = isChar = false; }
        void setInt() { isInt = true; isChar = isVoid = false; }
        void setChar() { isChar = true; isInt = isVoid = false; }
    };

    class FuncFParams: public Node {
    public:
        explicit FuncFParams(const Token& t) : Node(t) {}
        vector<u_ptr<FuncFParam>> funcFParams;
    };

    class FuncFParam: public Node {
    public:
        explicit FuncFParam(const Token& t) : Node(t) {}
        u_ptr<BType> bType;
        u_ptr<Ident> ident;
        bool isArray;
    };

    class Block: public Node {
    public:
        explicit Block(const Token& t) : Node(t) {}
        u_ptr<BlockItem> blockItem;

    };

    using BlockItemVariant = variant<u_ptr<Decl>, u_ptr<Stmt>>;
    class BlockItem: public Node {
    public:
        explicit BlockItem(const Token& t) : Node(t) {}
        u_ptr<BlockItemVariant> blockItem;
    };

    class Assignment: public Node { // this is the ForStmt in the document
    public:
        explicit Assignment(const Token& t) : Node(t) {}
        u_ptr<LVal> lVal;
        u_ptr<Exp> exp;

    };

    class AssignStmt: public Node {
    public:
        explicit AssignStmt(const Token& t) : Node(t) {}
        u_ptr<Assignment> assignment;
    };

    class IfStmt: public Node {
    public:
        explicit IfStmt(const Token& t) : Node(t) {}
        u_ptr<Cond> cond;
        u_ptr<Stmt> ifStmt;
        u_ptr<Stmt> elseStmt;
    };

    class ForStmt: public Node { // this is NOT the ForStmt in the document
    public:
        explicit ForStmt(const Token& t) : Node(t) {}
        u_ptr<Assignment> init;
        u_ptr<Cond> cond;
        u_ptr<Assignment> update;
        u_ptr<Stmt> stmt;
    };

    class BreakStmt: public Node {

    explicit BreakStmt(const Token& t) : Node(t) {}
    };

    class ContinueStmt: public Node {

    explicit ContinueStmt(const Token& t) : Node(t) {}
    };

    class ReturnStmt: public Node {
    public:
        explicit ReturnStmt(const Token& t) : Node(t) {}
        u_ptr<Exp> exp;
    };

    class GetintStmt: public Node {
    public:
        explicit GetintStmt(const Token& t) : Node(t) {}
        u_ptr<LVal> lVal;
    };

    class GetcharStmt: public Node {
    public:
        explicit GetcharStmt(const Token& t) : Node(t) {}
        u_ptr<LVal> lVal;
    };

    class PrintfStmt: public Node {
    public:
        explicit PrintfStmt(const Token& t) : Node(t) {}
        u_ptr<StringConst> stringConst;
        vector<u_ptr<Exp>> exps;
    };

    using StmtVariant = variant<u_ptr<AssignStmt>, u_ptr<Exp>, u_ptr<Block>,
                                u_ptr<IfStmt>, u_ptr<ForStmt>, u_ptr<BreakStmt>,
                                u_ptr<ContinueStmt>, u_ptr<ReturnStmt>, u_ptr<GetintStmt>,
                                u_ptr<GetcharStmt>, u_ptr<PrintfStmt>>;
    class Stmt: public Node {
    public:
        explicit Stmt(const Token& t) : Node(t) {}
        u_ptr<StmtVariant> stmt;
    };

    class Exp: public Node {
    public:
        explicit Exp(const Token& t) : Node(t) {}
        u_ptr<AddExp> addExp;
    };

    class Cond: public Node {
    public:
        explicit Cond(const Token& t) : Node(t) {}
        u_ptr<LOrExp> lOrExp;
    };

    class LVal: public Node {
    public:
        explicit LVal(const Token& t) : Node(t) {}
        u_ptr<Ident> ident;
        u_ptr<Exp> exp;
    };

    using PrimaryExpVariant = variant<u_ptr<Exp>, u_ptr<LVal>, u_ptr<Number>, u_ptr<Character>>;
    class PrimaryExp: public Node {
    public:
        explicit PrimaryExp(const Token& t) : Node(t) {}
        u_ptr<PrimaryExpVariant> primaryExp;
    };

    class Number: public Node {
    public:
        explicit Number(const Token& t) : Node(t) {}
        u_ptr<IntConst> intConst;
    };

    class Character: public Node {
    public:
        explicit Character(const Token& t) : Node(t) {}
        u_ptr<CharConst> charConst;
    };

    class FuncCall: public Node {
    public:
        explicit FuncCall(const Token& t) : Node(t) {}
        u_ptr<Ident> ident;
        u_ptr<FuncRParams> funcRParams;
    };

    using UnaryExpVariant = variant<u_ptr<PrimaryExp>, u_ptr<FuncCall>>;
    class UnaryExp: public Node {
    public:
        explicit UnaryExp(const Token& t) : Node(t) {}
        vector<u_ptr<UnaryOp>> unaryOps;
        u_ptr<UnaryExpVariant> unaryExp;

    };

    class UnaryOp: public Node {
    public:
        explicit UnaryOp(const Token& t) : Node(t) {}
        bool isPlus;
        bool isMinus;
        bool isExc;
        void setPlus() { isPlus = true; isMinus = isExc = false; }
        void setMinus() { isMinus = true; isPlus = isExc = false; }
        void setExc() { isExc = true; isMinus = isPlus = false; }
    };

    class FuncRParams: public Node {
    public:
        explicit FuncRParams(const Token& t) : Node(t) {}
        vector<u_ptr<Exp>> exps;
    };

    class MulExp: public Node {
    public:
        explicit MulExp(const Token& t) : Node(t) {}
        vector<u_ptr<UnaryExp>> unaryExps;
        vector<Token> ops;
    };

    class AddExp: public Node {
    public:
        explicit AddExp(const Token& t) : Node(t) {}
        vector<u_ptr<MulExp>> mulExps;
        vector<Token> ops;
    };

    class RelExp: public Node {
    public:
        explicit RelExp(const Token& t) : Node(t) {}
        vector<u_ptr<AddExp>> addExps;
        vector<Token> ops;
    };

    class EqExp: public Node {
    public:
        explicit EqExp(const Token& t) : Node(t) {}
        vector<u_ptr<RelExp>> relExps;
        vector<Token> ops;
    };

    class LAndExp: public Node {
    public:
        explicit LAndExp(const Token& t) : Node(t) {}
        vector<u_ptr<EqExp>> eqExps;
    };

    class LOrExp: public Node {
    public:
        explicit LOrExp(const Token& t) : Node(t) {}
        vector<u_ptr<LAndExp>> lAndExps;
    };

    class ConstExp: public Node {
    public:
        explicit ConstExp(const Token& t) : Node(t) {}
        u_ptr<AddExp> addExp;
    };

    class Ident: public Node {
    public:
        explicit Ident(const Token& t) : Node(t) {}
        std::string str;

    };

    class IntConst: public Node {
    public:
        explicit IntConst(const Token& t) : Node(t) {}
        int val;
    };

    class CharConst: public Node {
    public:
        explicit CharConst(const Token& t) : Node(t) {}
        char ch;

    };

    class StringConst: public Node {
    public:
        explicit StringConst(const Token& t) : Node(t) {}
        u_ptr<std::string> str;

    };
} // namespace tang

#endif //AST_HPP
