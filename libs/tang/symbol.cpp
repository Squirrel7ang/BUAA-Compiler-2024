//
// Created by tang on 10/23/24.
//

#include "symbol.hpp"

namespace tang {
    bool SymbolTable::_isDuplicatedSymbol(const unsigned int lin, Symbol& s) {
        Symbol _s = Symbol();;
        if (findSymbolLocal(_s, s.getName())) {
            _reporter.report(lin, 'b');
            return true;
        }
        return false;
    }

    void SymbolTable::addSymbol(const unsigned int lin, Symbol& s) {
        if (!_isDuplicatedSymbol(lin, s)) {
            _symbolStack.push_back(s);
        }
    }

    unsigned int SymbolTable::enterScope() {
        _scopePtrs.push_back(_symbolStack.size());

        std::vector<Symbol> curScope;
        _OutputSymbolStack.push_back(curScope);

        return (int)_OutputSymbolStack.size() - 1;
    }

    void SymbolTable::exitScope(unsigned int index) {
        std::vector<Symbol>& curScope = _OutputSymbolStack.at(index);
        int begin = _scopePtrs.back();
        int end = _symbolStack.size();
        for (int i = begin; i < end; i++) {
            curScope.push_back(_symbolStack.at(i));
        }
        for (int i = begin; i < end; i++) {
            _symbolStack.pop_back();
        }

        _scopePtrs.pop_back();
        _curPtr = _scopePtrs.back();
    }

    bool SymbolTable::findSymbolLocal(Symbol& s, const std::string& name) {
        for (unsigned int i = _scopePtrs.back(); i < _symbolStack.size(); i++) {
            if (name == _symbolStack.at(i).getName()) {
                s = _symbolStack.at(i);
                return true;
            }
        }
        return false;
    }

    bool SymbolTable::findSymbolGlobal(Symbol& s, const std::string& name) {
        for (int i = _symbolStack.size() - 1; i >= 0; i--) {
            auto& _s = _symbolStack.at(i);
            if (name == _s.getName()) {
                s = _s;
                return true;
            }
        }
        return false;
    }

    void SymbolTable::print() {
        int end = _OutputSymbolStack.size() - 1;
        for (int i = 0; i < _OutputSymbolStack.size(); i++) {
            auto curScope = _OutputSymbolStack.at(i);
            for (auto symbol: curScope) {
                _out << i+1 << " " << symbol.getName() << " " << symbol.getType()->toOutput() << std::endl;
            }
        }
    }
} // namespace tang
