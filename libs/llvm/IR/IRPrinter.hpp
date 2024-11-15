//
// Created by tang on 11/15/24.
//

#ifndef IRPRINTER_HPP
#define IRPRINTER_HPP
#include <ostream>

namespace llvm {
    class IRPrinter {
        std::ostream& _out;
    public:
        explicit IRPrinter(std::ostream& out) : _out(out) { }
        void print(int i) { _out << i; }
        void print(std::string& s) { _out << s; }
        void print(char ch) { _out << ch; }
    };
}

#endif //IRPRINTER_HPP
