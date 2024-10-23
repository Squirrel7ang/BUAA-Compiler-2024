//
// Created by tang on 10/23/24.
//

#include "symbol.hpp"

namespace tang {
    bool SymbolTable::_checkDuplicatedSymbol(const unsigned int lin, Symbol& s) {
        Symbol _s = Symbol();;
        if (!findSymbolLocal(_s, s.getName())) {
            _reporter.report(lin, 'b');
        }
    }

    void SymbolTable::addSymbol(const unsigned int lin, Symbol& s) {
        if (!_checkDuplicatedSymbol(lin, s)) {
            _symbolStack.push_back(s);
        }
    }

    void SymbolTable::enterScope() {
        _scopePtrs.push_back(_symbolStack.size());
    }

    void SymbolTable::exitScope() {
        std::vector<Symbol> curScope;
        for (unsigned int i = _scopePtrs.back(); i < _symbolStack.size(); i++) {
            curScope.push_back(_symbolStack.at(i));
        }
        _OutputSymbolStack.push_back(curScope);

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
        for (auto& _s: _symbolStack) {
            if (name == _s.getName()) {
                s = _s;
                return true;
            }
        }
        return false;
    }

    void SymbolTable::print() {
        for (int i = 0; i < _OutputSymbolStack.size(); i++) {
            auto curScope = _OutputSymbolStack.at(i);
            for (auto symbol: curScope) {
                _out << i+1 << " " << symbol.getName() << " " << symbol.getType().toOutput() << std::endl;
            }
        }
    }
} // namespace tang
