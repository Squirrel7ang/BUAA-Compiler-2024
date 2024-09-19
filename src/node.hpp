#ifndef _NODE_HPP
#define _NODE_HPP

#include <vector>
#include "token.hpp"

namespace expr {
    class Node {
    
    public:
        virtual int get_value();
    };

    class Poly : Node {
        std::vector <Term> terms;
        std::vector <Token> ops;
    public:
        explicit Poly(Term& t);
        void add_term(Term& t, Token& op);
        int get_value();
    };

    class Term : Node {
        std::vector<Number> numbers;
        std::vector <Token> ops;
    
    public:
        explicit Term(Number n);
        void add_number(Number n, Token op);
        int get_value();
    };

    class Number : Node {
        int value;
    public: 
        explicit Number(int num);
        int get_value();
    };
}

#endif // _NODE_HPP