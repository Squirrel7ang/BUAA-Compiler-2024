//
// Created by tang on 11/26/24.
//

#include "TmpRegTable.hpp"

#include "Regs.hpp"
#include "VarTable.hpp"

namespace mips {
    TmpRegTablePtr TmpRegTable::New() {
        return std::make_shared<TmpRegTable>();
    }

    unsigned int TmpRegTable::size() {
        return _regPool.size();
    }

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

    void TmpRegTable::checkRefCount() {
        for (auto& pair: _regPool) {
            auto var = pair.second;
            if (var != nullptr && var->reachRefCount()) {
                pair.second = nullptr;
            }
        }
    }

    bool TmpRegTable::isFree() {
        checkRefCount();
        for (auto& pair: _regPool) {
            if (pair.second == nullptr) {
                return true;
            }
        }

        return false;
    }

    MipsRegPtr TmpRegTable::allocateReg() {
        checkRefCount();
        for (auto& pair: _regPool) {
            if (pair.second == nullptr) {
                return pair.first;
            }
        }
        return nullptr;
    }

    void TmpRegTable::occupyReg(VariablePtr var, MipsRegPtr reg) {
    }

}
