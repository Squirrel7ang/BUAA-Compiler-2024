//
// Created by tang on 10/13/24.
//

#ifndef AST_HPP
#define AST_HPP

#include <memory>

namespace tang {
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
        std::vector<std::unique_ptr<Decl>> _decls;
        std::vector<std::unique_ptr<FuncDef>> _funcdefs;
        std::unique_ptr<MainFuncDef> _mainFuncDef;

    public:
        void addDecl(const std::unique_ptr<Decl> &decl) { _decls.push_back(decl); }
        void addFuncDef(const std::unique_ptr<FuncDef> &funcdef) { _funcdefs.push_back(funcdef); }
        void addMainFuncDef(const std::unique_ptr<MainFuncDef> &mainFuncDef) {
            // TODO
        }

    };

    class Decl: public Node {

    };

    class ConstDecl: public Node {

    };

    class BType: public Node {

    };

    class ConstDef: public Node {

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

    class FuncParams: public Node {

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
