//
// Created by tang on 11/26/24.
//

#ifndef SAVEREGTABLE_HPP
#define SAVEREGTABLE_HPP

#include <map>

#include "Regs.hpp"
#include "IR/Common.hpp"

namespace mips {
    class SaveRegTable {
        std::map<MipsRegPtr, llvm::InstructionPtr> _regTable;
    public:
        explicit SaveRegTable();
        bool isFree();
        MipsRegPtr allocateReg();
        void insert(VariablePtr inst, MipsRegPtr reg);
    };
}

#endif //SAVEREGTABLE_HPP
