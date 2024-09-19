#include "node.hpp"
#include "token.hpp"

namespace expr {
    int Poly::get_value() {
        int ret = terms[0].get_value();
        for (int i = 0; i < ops.size(); i++) {
            if (ops[i].euqals(TokenType::ADD)) {
                ret += terms[i+1].get_value();
            }
            else if (ops[i].get_type() == TokenType::SUB) {

            }
        }

    }

    Poly::Poly(Term& t) {
        this->terms.push_back(t);
    }

    void Poly::add_term(Term& t, Token& op) {
        this->terms.push_back(t);
        this->ops.push_back(op);
    }

    int Term::get_value() {
        

    }

    int Number::get_value() {
        return this->value;
    }
}