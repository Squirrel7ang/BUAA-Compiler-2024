//
// Created by tang on 11/26/24.
//

#ifndef VARTABLE_HPP
#define VARTABLE_HPP

#include <map>

#include "MipsCommon.hpp"
#include "IR/Common.hpp"
#include "IR/Instruction.hpp"

namespace mips {
    class Variable {
        // llvm
        llvm::InstructionPtr _llvmInst;

        // reference count
        const unsigned int _totalCounts;
        unsigned int _refCounts;

        // storage
        MipsRegPtr _reg;
        StackSlotPtr _slot;

        // attributes
        bool isTemp; // store in t0-t9 or stack/frame
        bool isSave; // store in s0-s7 or stack/frame
    public:
        explicit Variable(llvm::InstructionPtr inst, unsigned int totalUse, MipsRegPtr& reg);

        explicit Variable(llvm::InstructionPtr inst, unsigned int totalUse, StackSlotPtr &slot);

        void count() {
            _refCounts++;
            assert(_refCounts <= _totalCounts);
        }
        bool reachTotalUse() {
            return _refCounts == _totalCounts;
        }
        MipsRegPtr getReg() { return _reg; }
    private:
        void setReg(MipsRegPtr& reg) {
            _reg = reg;
        }
    };

    class VarTable {
    private:
        std::map<llvm::InstructionPtr, VariablePtr> _varTable;
    public:
        MipsRegPtr getMipsReg(llvm::InstructionPtr vp);
        void countUse(llvm::InstructionPtr vp);
    };
}

#endif //VARTABLE_HPP
