//
// Created by tang on 11/26/24.
//

#include <cassert>

#include "VarTable.hpp"

#include "IR/Instructions.hpp"

namespace mips {
    Variable::Variable(llvm::InstructionPtr inst, unsigned int totalUse, MipsRegPtr &reg)
        : _llvmInst(inst), _totalCounts(totalUse), _refCounts(0), _reg(reg) {
        // TODO: isTemp isSave
    }

    Variable::Variable(llvm::InstructionPtr inst, unsigned int totalUse, StackSlotPtr &slot)
        : _llvmInst(inst), _totalCounts(totalUse), _refCounts(0), _slot(slot) {
        // TODO: isTemp isSave
    }

    MipsRegPtr VarTable::getMipsReg(llvm::InstructionPtr ip) {
        if (_varTable.contains(ip)) {
            return _varTable.at(ip)->getReg(); // !!! this could also be nullptr
        }
        else {
            return nullptr;
        }
    }

    void VarTable::countUse(llvm::InstructionPtr ip) {
        _varTable.at(ip)->count();
    }

    // void VarTable::allocateReg(llvm::InstructionPtr ip, MipsRegPtr reg) {
    //     _varTable.at(ip)->setReg(reg);
    // }
}
