#ifndef _NODE_H
#define _NODE_H

#include <vector>
#include "token.h"

namespace node {
    class Node {
        Node& parent;

    public:
        explicit Node(Node & parent) :
            parent(parent) {}
    };

    class SentenceNode: Node {
        std::vector<TermNode&> terms;
        std::vector<token::token_type> ops;

    public:
        explicit SentenceNode(Node& parent, TermNode& term);
        explicit SentenceNode(TermNode& term);
        void push_term(TermNode& term, token::token_type op);
        int get_val();
    };

    class TermNode : Node {
        std::vector<FactorNode&> factors;
        std::vector<token::token_type> ops;

    public:
        explicit TermNode(Node& parent, FactorNode& factor);
        void push_factor(FactorNode& factor, token::token_type op);
        int get_val();
    };

    class FactorNode : Node {
        std::vector<NumberNode&> numbers;
    public:
        explicit FactorNode(Node& parent, NumberNode& number);
        void push_number(int number);
        int get_val();
    };

    class NumberNode : Node {
        int num;

    public:
        explicit NumberNode(Node& parent, int num);
        int get_val();
    };

} // namespace node

#endif // _NODE_H