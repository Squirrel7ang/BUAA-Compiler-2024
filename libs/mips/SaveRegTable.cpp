//
// Created by tang on 12/6/24.
//

#include "SaveRegTable.hpp"
#include "Regs.hpp"

namespace mips {
    SaveRegTable::SaveRegTable()
        : _saveRegPool({REG_S0,
            REG_S1,
            REG_S2,
            REG_S3,
            REG_S4,
            REG_S5,
            REG_S6,
            REG_S7}),
        _used({{REG_S0, false},
            {REG_S1, false},
            {REG_S2, false},
            {REG_S3, false},
            {REG_S4, false},
            {REG_S5, false},
            {REG_S6, false},
            {REG_S7, false}}) {

    }

    SaveRegTablePtr SaveRegTable::New() {
        return std::make_shared<SaveRegTable>();
    }

    int SaveRegTable::size() {
        return _saveRegPool.size();
    }

    void SaveRegTable::use(const MipsRegPtr &reg) {

    }
}
