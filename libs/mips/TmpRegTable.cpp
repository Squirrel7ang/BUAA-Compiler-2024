//
// Created by tang on 11/26/24.
//

#include "TmpRegTable.hpp"

#include "Regs.hpp"

namespace mips {
    TmpRegTable::TmpRegTable()
        : _regPool({
            {REG_T0, nullptr},
            {REG_T1, nullptr},
            {REG_T2, nullptr},
            {REG_T3, nullptr},
            {REG_T4, nullptr},
            {REG_T5, nullptr},
            {REG_T6, nullptr},
            {REG_T7, nullptr},
            {REG_T8, nullptr},
            {REG_T9, nullptr} }) { }

    bool TmpRegTable::isFree() {
        for (auto& pair: _regPool)
            if (pair.second == nullptr)
                return true;
        return false;
    }

    MipsRegPtr TmpRegTable::allocateReg() {
        for (auto& pair: _regPool)
            if (pair.second == nullptr)
                return pair.first;
        return nullptr;
    }

    void TmpRegTable::insert(VariablePtr var, MipsRegPtr reg) {

    }

}
