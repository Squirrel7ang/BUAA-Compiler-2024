//
// Created by tang on 11/25/24.
//

#include "Ast.hpp"

namespace tang {
    s_ptr<SymbolType> StringConst::symbolTypeAt(unsigned int i) {
        return _types.at(i);
    }

    void StringConst::calFormat() {
        unsigned int ret = 0;
        for (int i = 0; i < _str.length()-1; i++) {
            char ch1 = _str.at(i);
            char ch2 = _str.at(i+1);
            if (ch1 == '%' && ch2 == 'd') {
                auto sty = std::make_shared<IntSymbolType>(false);
                _types.push_back(sty);
            }
            if (ch1 == '%' && ch2 == 'c') {
                auto sty = std::make_shared<CharSymbolType>(false);
                _types.push_back(sty);
            }
        }
    }

    s_ptr<SymbolType> PrintfStmt::symbolTypeAt(unsigned int i) {
        return stringConst->symbolTypeAt(i);
    }
}
