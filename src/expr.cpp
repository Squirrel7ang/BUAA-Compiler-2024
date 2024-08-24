#include "expr.hpp"

namespace expr {
    int Expr::get_value() {
        Lexer l = Lexer(str);
        Parser p = Parser(l);
        return p.get_value();
    }
}