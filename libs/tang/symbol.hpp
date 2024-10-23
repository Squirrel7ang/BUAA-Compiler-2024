//
// Created by tang on 10/19/24.
//

#ifndef SYMBOL_HPP
#define SYMBOL_HPP
#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "error.hpp"

namespace tang {
    class SymbolType {
    public:
        virtual ~SymbolType();
        virtual unsigned int getSize();
        virtual std::string toOutput();
        [[nodiscard]] virtual bool isConst() const ;
    };

    class IntSymbolType: public SymbolType {
        const bool _isConst;
    public:
        explicit IntSymbolType(bool isConst): _isConst(isConst) {}
        unsigned int getSize() override {
            return 4;
        }
        std::string toOutput() override {
            if (_isConst)
                return "ConstInt";
            else
                return "Int";
        }
        [[nodiscard]] bool isConst() const override {
            return _isConst;
        }
    };

    class CharSymbolType: public SymbolType {
        const bool _isConst;
    public:
        explicit CharSymbolType(bool isConst): _isConst(isConst) {}
        unsigned int getSize() override {
            return 1;
        }
        std::string toOutput() override {
            if (_isConst)
                return "ConstChar";
            else
                return "Char";
        }
        [[nodiscard]] bool isConst() const override {
            return _isConst;
        }
    };

    class VoidSymbolType: public SymbolType {
    public:
        unsigned int getSize() override {
            assert(0);
        }
        std::string toOutput() override {
            return "Void";
        }
        [[nodiscard]] bool isConst() const override {
            assert(0);
        }
    };

    class IntArrayType: public SymbolType {
        const bool _isConst;
    public:
        const unsigned int _len;
        explicit IntArrayType(bool isConst, const unsigned int len) 
            : _isConst(isConst), _len(len) { }
        unsigned int getSize() override {
            return _len << 2;
        }
        std::string toOutput() override {
            if (_isConst)
                return "ConstIntArray";
            else 
                return "IntArray";
        }
        [[nodiscard]] bool isConst() const override {
            return _isConst;
        }
    };

    class CharArrayType: public SymbolType {
        const bool _isConst;
    public:
        const unsigned int _len;
        explicit CharArrayType(const bool isConst, const unsigned int len) 
            : _isConst(isConst), _len(len) {}
        unsigned int getSize() override {
            return _len << 2;
        }
        std::string toOutput() override {
            if (_isConst)
                return "ConstCharArray";
            else 
                return "CharArray";
        }
        [[nodiscard]] bool isConst() const override {
            return _isConst;
        }
    };

    class FuncSymbolType: public SymbolType {
    public:
        // explicit FuncSymbolType() = default; // TODO
        explicit FuncSymbolType(SymbolType& returnType) :_returnType(returnType) {}
        SymbolType _returnType;
        std::vector<SymbolType> _argType;
        unsigned int getSize() override {
            assert(0);
        }
        std::string toOutput() override {
            return _returnType.toOutput() + "Func";
        }
        void addArgType(const SymbolType& argType) {
            _argType.push_back(argType);
        }
        [[nodiscard]] bool isConst() const override {
            assert(0);
        }
    };

    class Symbol {
        SymbolType _type;
        std::string _name;
    public:
        explicit Symbol() = default;
        explicit Symbol(const SymbolType& type, std::string  name)
            :_type(type), _name(std::move(name)) { }
        SymbolType getType() {
            return _type;
        }
        std::string getName() {
            return _name;
        }
    };

    class SymbolTable {
    private:
        std::vector<Symbol> _symbolStack;
        std::vector<unsigned int> _scopePtrs;
        std::vector<std::vector<Symbol>> _OutputSymbolStack; // use for output and nothing else;
        std::ostream& _out;
        ErrorReporter& _reporter;
        unsigned int _curPtr;
        bool _checkDuplicatedSymbol(unsigned int, Symbol& );
    public:
        explicit SymbolTable(std::ostream& out, ErrorReporter& reporter) 
            : _out(out), _reporter(reporter), _curPtr(0) {

        }
        void addSymbol(unsigned int, Symbol&);
        Symbol popSymbol() {
            auto&& s = _symbolStack.back();
            _symbolStack.pop_back();
            return s;
        }
        void enterScope();
        void exitScope();
        bool findSymbolGlobal(Symbol& s, const std::string& name);
        bool findSymbolLocal(Symbol &s, const std::string &name);
        void print();
    };
} // namespace tang

#endif //SYMBOL_HPP
