//
// Created by tang on 12/6/24.
//

#include "SaveRegTable.hpp"

namespace mips {
    SaveRegTable::SaveRegTable()
        : _regTable({
            {REG_S0, nullptr},
            {REG_S1, nullptr},
            {REG_S2, nullptr},
            {REG_S3, nullptr},
            {REG_S4, nullptr},
            {REG_S5, nullptr},
            {REG_S6, nullptr},
            {REG_S7, nullptr} }) { }

    bool SaveRegTable::isFree() {
        for (auto& pair: _regTable)
            if (pair.second == nullptr)
                return true;
        return false;
    }

    MipsRegPtr SaveRegTable::allocateReg() {
    }

    void SaveRegTable::insert(VariablePtr inst, MipsRegPtr reg) {
    }
}
