#ifndef _EXPR_HPP
#define _EXPR_HPP

#include <string>

namespace expr {

    class Expr {
        std::string str;
        bool calculated;
        int value;
    public:
        Expr(std::string str) : str(str) {
            value = 0;
            calculated = false;
        }
        int get_value();
    };
}

#endif // _EXPR_HPP