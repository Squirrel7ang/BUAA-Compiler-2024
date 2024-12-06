//
// Created by tang on 11/26/24.
//

#ifndef TMPREGTABLE_HPP
#define TMPREGTABLE_HPP

#include <map>

#include "MipsCommon.hpp"
#include "IR/Common.hpp"

namespace mips {
    class TmpRegTable {
        std::map<MipsRegPtr, VariablePtr> _regPool;
    public:
        explicit TmpRegTable();
        bool isFree();
        MipsRegPtr allocateReg();

        void insert(VariablePtr var, MipsRegPtr reg);
    };
}

#endif //TMPREGTABLE_HPP
