
/**
 * only support + - * / 
 */

#include <string>
#include <iostream>

#include "expr.hpp"

using namespace expr;

int main() {
    std::string s;
    while (1) {
        std::cout << "enter a expression: ";
        std::cin >> s;
        if (s[0] == 'q') {
            exit(0);
        }

        Expr e = Expr(s);
        std::cout << "value of this expression is " << e.get_value() << " " << std::endl;
    }
}