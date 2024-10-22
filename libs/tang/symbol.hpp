//
// Created by tang on 10/19/24.
//

#ifndef SYMBOL_HPP
#define SYMBOL_HPP
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "error.hpp"

namespace tang {
    class SymbolType {
    public:
        unsigned int getSize();
        std::string toOutput();
    };

    class IntSymbolType: public SymbolType {
    public:
        explicit IntSymbolType(bool isConst): _isConst(isConst) {}
        bool _isConst;
        unsigned int getSize() {
            return 4;
        }
        std::string toOutput() {
            if (_isConst)
                return "ConstInt";
            else
                return "Int";
        }
    };

    class CharSymbolType: public SymbolType {
    public:
        explicit CharSymbolType(bool isConst): _isConst(isConst) {}
        bool _isConst;
        unsigned int getSize() {
            return 1;
        }
        std::string toOutput() {
            if (_isConst)
                return "ConstChar";
            else
                return "Char";
        }
    };

    class VoidSymbolType: public SymbolType {
    public:
        unsigned int getSize();
        std::string toOutput() {
            return "Void";
        }
    };

    class IntArrayType: public SymbolType {
    public: 
        const bool _isConst;
        const unsigned int _len;
        explicit IntArrayType(bool isConst, const unsigned int len) 
            : _isConst(isConst), _len(len) { }
        unsigned int getSize() {
            return _len << 2;
        }
        std::string toOutput() {
            if (_isConst)
                return "ConstIntArray";
            else 
                return "IntArray";
        }
    };

    class CharArrayType: public SymbolType {
    public:
        const bool _isConst;
        const unsigned int _len;
        explicit CharArrayType(const bool isConst, const unsigned int len) 
            : _isConst(isConst), _len(len) {}
        unsigned int getSize() {
            return _len << 2;
        }
        std::string toOutput() {
            if (_isConst)
                return "ConstCharArray";
            else 
                return "CharArray";
        }
    };

    class FunctionType: public SymbolType {
    public:
        explicit FunctionType(SymbolType& returnType) :_returnType(returnType) {}
        SymbolType _returnType;
        std::vector<SymbolType> _argType;
        unsigned int getSize() {
            assert(0);
        }
        std::string toOutput() {
            return _returnType.toOutput() + "Func";
        }
        void addArgType(SymbolType& argType) {
            _argType.push_back(argType);
        }
    };

    class Symbol {
        SymbolType _type;
        std::string _name;
    public:
        explicit Symbol() {}
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
    private:
        std::vector<Symbol> _stack;
        std::vector<unsigned int> _scopePtrs;
        std::vector<std::vector<Symbol>> _globalStack; // use for output and nothing else;
        std::ostream& _out;
        ErrorReporter& _reporter;
        unsigned int _curPtr;
    public:
        explicit SymbolTable(std::ostream& out, ErrorReporter& reporter) 
            : _out(out), _curPtr(0), _reporter(reporter) {

        }
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
            std::vector<Symbol> curScope;
            for (unsigned int i = _scopePtrs.back(); i < _stack.size(); i++) {
                curScope.push_back(_stack.at(i));
            }
            _globalStack.push_back(curScope);

            _scopePtrs.pop_back();
            _curPtr = _scopePtrs.back();
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
        void print() {
            for (int i = 0; i < _globalStack.size(); i++) {
                auto curScope = _globalStack.at(i);
                for (auto symbol: curScope) {
                    _out << i+1 << " " << symbol.getName() << " " << symbol.getType().toOutput() << std::endl;
                }
            }
        }
    };
} // namespace tang

#endif //SYMBOL_HPP
