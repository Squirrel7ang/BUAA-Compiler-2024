//
// Created by tang on 11/26/24.
//

#ifndef SAVEREGTABLE_HPP
#define SAVEREGTABLE_HPP

#include <map>
#include <set>

#include "Regs.hpp"
#include "IR/Common.hpp"

namespace mips {
    class SaveRegTable {
    public:
        const std::set<MipsRegPtr> _saveRegPool;
        const std::map<MipsRegPtr, bool> _used;
        explicit SaveRegTable();
        static SaveRegTablePtr New();
        int size();
        void use(const MipsRegPtr & reg);
    };
}

#endif //SAVEREGTABLE_HPP
