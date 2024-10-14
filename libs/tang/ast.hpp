//
// Created by tang on 10/13/24.
//

#ifndef AST_HPP
#define AST_HPP

#include <memory>

namespace tang {
    class VarDecl;
    class ConstDecl;
    template <typename T>
    using u_ptr = std::unique_ptr<T>;

    template <typename T>
    using vector = std::vector<T>;
    class ConstInitVal;
    class ConstExp;
    class Ident;
    class MainFuncDef;
    class FuncDef;
    class Decl;

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

    class CompUnit: public Node {
        vector<u_ptr<Decl>> _decls;
        vector<u_ptr<FuncDef>> _funcdefs;
        u_ptr<MainFuncDef> _mainFuncDef;

    public:
        void addDecl(u_ptr<Decl> &decl) { _decls.push_back(std::move(decl)); }
        void addFuncDef(u_ptr<FuncDef> &funcdef) { _funcdefs.push_back(std::move(funcdef)); }
        void addMainFuncDef(u_ptr<MainFuncDef> &mainFuncDef) {
            _mainFuncDef = std::move(mainFuncDef);
        }
    };

    class Decl: public Node {
        u_ptr<ConstDecl> constDecl;
        u_ptr<VarDecl> varDecl;
        bool isConstDecl;
        bool isVarDecl;


    };

    class ConstDecl: public Node {

    };

    class BType: public Node {
        bool _isInt;
        bool _isChar;
    public:
        void addInt() { _isInt = true, _isChar = false; }
        void addChar() { _isChar = true, _isInt = false; }
    };

    class ConstDef: public Node {
        u_ptr<Ident> _ident;
        u_ptr<ConstExp> _constExp;
        u_ptr<ConstInitVal> _constInitVal;
    public:
        void addIdent()


    };

    class ConstInitVal: public Node {

    };

    class VarDecl: public Node {

    };

    class VarDef: public Node {

    };

    class InitVal: public Node {

    };

    class FuncDef: public Node {

    };

    class MainFuncDef: public Node {

    };

    class FuncType: public Node {

    };

    class FuncFParams: public Node {

    };

    class FuncFParam: public Node {

    };

    class Block: public Node {

    };

    class BlockItem: public Node {

    };

    class Stmt: public Node {

    };

    class ForStmt: public Node {

    };

    class Exp: public Node {

    };

    class Cond: public Node {

    };

    class LVal: public Node {

    };

    class PrimaryExp: public Node {

    };

    class Number: public Node {

    };

    class Character: public Node {

    };

    class UnaryExp: public Node {

    };

    class UnaryOp: public Node {

    };

    class FuncRParams: public Node {

    };

    class MulExp: public Node {

    };

    class AddExp: public Node {

    };

    class RelExp: public Node {

    };

    class EqExp: public Node {

    };

    class LAndExp: public Node {

    };

    class LOrExp: public Node {

    };

    class ConstExp: public Node {

    };

    class Ident: public Node {

    };

    class IntConst: public Node {

    };

    class CharConst: public Node {

    };

    class StringConst: public Node {

    };
} // namespace tang

#endif //AST_HPP
