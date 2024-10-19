//
// Created by tang on 10/19/24.
//

#ifndef SYMBOL_HPP
#define SYMBOL_HPP
#include <cassert>
#include <string>
#include <vector>

namespace llvm {
    class SymbolType {

    public:
        unsigned int getSize();
    };

    class IntSymbolType: SymbolType {
    public:
        unsigned int getSize() {
            return 4;
        }

    };

    class CharSymbolType: SymbolType {
    public:
        unsigned int getSize() {
            return 1;
        }
    };

    class VoidSymbolType: SymbolType {
    public:
        unsigned int getSize() {
            assert(0);
        }
    };

    class ArraySymbolType: SymbolType {
        const SymbolType _basicType;
        const unsigned int _length;
        const unsigned int _size;
    public:
        explicit ArraySymbolType(SymbolType& basicType, const unsigned int length)
            : _basicType(basicType), _length(length), _size(basicType.getSize() * length) { }
        SymbolType getType() {
            return _basicType;
        }
    };

    class Symbol {
        SymbolType _type;
        std::string _name;
    public:
        explicit Symbol(SymbolType type, std::string& name)
            :_type(type), _name(name) { }
        SymbolType getType() {
            return _type;
        }
        std::string getName() {
            return _name;
        }
    };

    class SymbolTable {
        std::vector<Symbol> _stack;
        std::vector<unsigned int> _scopePtrs;
    public:
        void addSymbol(Symbol& s) {
            _stack.push_back(s);
        }
        Symbol popSymbol() {
            auto&& s = _stack.back();
            _stack.pop_back();
            return s;
        }
        void enterScope() {
            _scopePtrs.push_back(_stack.size());
        }
        void exitScope() {
            _scopePtrs.pop_back();
        }
        bool findSymbol(Symbol& s, const std::string& name) {
            for (unsigned int i = _scopePtrs.back(); i < _stack.size(); i++) {
                if (name == _stack.at(i).getName()) {
                    s = _stack.at(i);
                    return true;
                }
            }
            return false;
        }
    };

} // namespace llvm

#endif //SYMBOL_HPP
