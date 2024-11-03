//
// Created by tang on 10/19/24.
//

#ifndef SYMBOL_HPP
#define SYMBOL_HPP
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "error.hpp"

namespace tang {
    template <typename T>
    using s_ptr = std::shared_ptr<T>;

    class RawSymbolType {
    public:
        int id;
        explicit RawSymbolType(const int i): id(i) {}
        bool operator==(const RawSymbolType & operand) const {
            return id == operand.id;
        }
    };
    static RawSymbolType INT_ST = RawSymbolType(1);
    static RawSymbolType CHAR_ST = RawSymbolType(2);
    static RawSymbolType CONST_INT_ST = RawSymbolType(3);
    static RawSymbolType CONST_CHAR_ST = RawSymbolType(4);
    static RawSymbolType INT_ARRAY_ST = RawSymbolType(5);
    static RawSymbolType CHAR_ARRAY_ST = RawSymbolType(6);
    static RawSymbolType CONST_INT_ARRAY_ST = RawSymbolType(7);
    static RawSymbolType CONST_CHAR_ARRAY_ST = RawSymbolType(8);
    static RawSymbolType VOID_FUNC_ST = RawSymbolType(9);
    static RawSymbolType INT_FUNC_ST = RawSymbolType(10);
    static RawSymbolType CHAR_FUNC_ST = RawSymbolType(11);
    static RawSymbolType VOID_ST = RawSymbolType(12);

    class SymbolType {
    public:
        virtual unsigned int getSize() = 0;
        virtual std::string toOutput() = 0;
        virtual bool isArray() = 0;
        virtual bool isFunc() = 0;
        virtual RawSymbolType toRawType() = 0;
        virtual bool isConst() = 0;
        bool operator==(SymbolType & op) {
            return this->toRawType() == op.toRawType() &&
                this->isArray() == op.isArray() &&
                this->isFunc() == op.isFunc() &&
                this->getSize() == op.getSize() &&
                this->isConst() == op.isConst();
        }
    };

    class IntSymbolType: public SymbolType {
        const bool _isConst;
    public:
        explicit IntSymbolType(): _isConst(false) {}
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
        bool isConst() override {
            return _isConst;
        }
        RawSymbolType toRawType() {
            if (_isConst) return CONST_INT_ST;
            else return INT_ST;
        }
        bool isArray() override { return false; }
        bool isFunc() override { return false; }
    };

    static IntSymbolType INT_TYPE = IntSymbolType(false);
    static IntSymbolType CONST_INT_TYPE = IntSymbolType(true);

    class CharSymbolType: public SymbolType {
        const bool _isConst;
    public:
        explicit CharSymbolType(): _isConst(false) {}
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
        bool isConst() override {
            return _isConst;
        }
        RawSymbolType toRawType() {
            if (_isConst) return CONST_CHAR_ST;
            else return CHAR_ST;
        }
        bool isArray() override { return false; }
        bool isFunc() override { return false; }
    };

    static CharSymbolType CHAR_TYPE = CharSymbolType(false);
    static CharSymbolType CONST_CHAR_TYPE = CharSymbolType(true);

    class VoidSymbolType: public SymbolType {
    public:
        unsigned int getSize() override {
            return 0;
        }
        std::string toOutput() override {
            return "Void";
        }
        bool isConst() override {
            return false;
        }
        RawSymbolType toRawType() {
            return VOID_ST;
        }
        bool isArray() override { return false; }
        bool isFunc() override { return false; }
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
        bool isConst() override {
            return _isConst;
        }
        RawSymbolType toRawType() {
            if (_isConst) return CONST_INT_ARRAY_ST;
            else return INT_ARRAY_ST;
        }
        bool isArray() override { return true; }
        bool isFunc() override { return false; }
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
        bool isConst() override {
            return _isConst;
        }
        RawSymbolType toRawType() {
            if (_isConst) return CONST_CHAR_ARRAY_ST;
            else return CHAR_ARRAY_ST;
        }
        bool isArray() override { return true; }
        bool isFunc() override { return false; }
    };

    class FuncSymbolType: public SymbolType {
    public:
        explicit FuncSymbolType() = default;
        explicit FuncSymbolType(s_ptr<SymbolType> returnType) :_returnType(std::move(returnType)) {}
        s_ptr<SymbolType> _returnType;
        std::vector<s_ptr<SymbolType>> _argType;
        unsigned int getSize() override {
            return 8;
        }
        std::string toOutput() override {
            std::string s;
            s = _returnType->toOutput();
            return s + "Func";
        }
        void addArgType(const s_ptr<SymbolType>& argType) {
            _argType.push_back(argType);
        }
        bool isConst() override {
            assert(0);
        }
        RawSymbolType toRawType() override {
            auto rawType = _returnType->toRawType();
            if (rawType == INT_ST) {
                return INT_FUNC_ST;
            }
            else if (rawType == CHAR_ST) {
                return CHAR_FUNC_ST;
            }
            else if (rawType == VOID_ST) {
                return VOID_FUNC_ST;
            }
            else {
                assert(0);
            }
        }
        bool isArray() override { return false; }
        bool isFunc() override { return true; }
    };

    class Symbol {
        s_ptr<SymbolType> _type;
        std::string _name;
    public:
        explicit Symbol() = default;
        explicit Symbol(const s_ptr<SymbolType>& type, std::string  name)
            :_type(type), _name(std::move(name)) { }
        s_ptr<SymbolType> getType() {
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
        [[nodiscard]] unsigned int enterScope();
        void exitScope(unsigned int index);
        bool findSymbolGlobal(Symbol& s, const std::string& name);
        bool findSymbolLocal(Symbol &s, const std::string &name);
        void print();
    };
} // namespace tang

#endif //SYMBOL_HPP
