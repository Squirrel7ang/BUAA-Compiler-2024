#ifndef _NODE_H
#define _NODE_H

#include <vector>

namespace node {

class Node {
    Node& parent;

public:
    explicit Node(Node & parent) :
        parent(parent) {}
};

class PolyNode: Node {
    std::vector<TermNode&> terms;

public:
    explicit PolyNode(Node& parent) :
        Node(parent) {}
    void add_term(TermNode& term);
};

class TermNode : Node {
    std::vector<FactorNode&> factors;

};

class FactorNode : Node {
public:
    explicit FactorNode(Node& parent) :
        Node(parent) {}
};

class NumberNode : FactorNode {
    int num;

public:
    explicit NumberNode(Node& parent) :
        FactorNode(parent) {}
    explicit NumberNode(Node& parent, int n) :
        FactorNode(parent),
        num(n) {}
    int get_num();
};

} // namespace node

#endif // _NODE_H