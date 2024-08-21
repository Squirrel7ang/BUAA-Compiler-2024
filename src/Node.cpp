#include "node.h"

namespace node {

    SentenceNode::SentenceNode(Node& parent, TermNode& term) : 
        Node(parent), terms(), ops() {
        terms.push_back(term);
    }

    SentenceNode::SentenceNode(TermNode& term) : 
        Node(*this), terms(), ops() {
        terms.push_back(term);
    }

    void
    SentenceNode::push_term(TermNode& term, token::token_type op) {
        terms.push_back(term);
        ops.push_back(op);
    }

    int
    SentenceNode::get_val() {
        int res = terms[0].get_val();
        int len = ops.size();
        for (int i = 0; i < len; i++) {
            if (ops[i] == token::ADD) { // TODO
                res += terms[i+1].get_val();
            }
            else if (ops[i] == token::SUB) {
                res -= terms[i+1].get_val();
            }
            else {
                throw "none ADD/SUB op detected in sentence";
            }
        }
        
    }

    TermNode::TermNode(Node& parent, FactorNode& factor) : 
        Node(parent), factors(), ops() {
        factors.push_back(factor);
    }

    FactorNode::FactorNode(Node& parent, NumberNode& number) : 
        Node(parent), numbers() {
        numbers.push_back(number);
    }

    NumberNode::NumberNode(Node& parent, int number) : 
        Node(parent) {
        num = number;
    }


} // namespace node