//
// Created by tang on 11/15/24.
//

#ifndef IRPRINTER_HPP
#define IRPRINTER_HPP
#include <ostream>

#include "Common.hpp"
#include "Module.hpp"

namespace llvm {
    class IRPrinter {
        std::ostream& _out;
    public:
        explicit IRPrinter(std::ostream& out) : _out(out) { }
        void print(int i) { _out << i; }
        void print(std::string& s) { _out << s; }
        void print(char ch) { _out << ch; }
        void printHeader(std::string&& sourceName) {
            _out << "; ModuleID = '" + sourceName + "'" << std::endl;
            _out << "source_filename = \"" + sourceName + "\"" << std::endl;
            _out << std::endl;
        }
        void print(ModulePtr module) {
            module->print(_out);
        }
    };
}

#endif //IRPRINTER_HPP
