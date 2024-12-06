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
        VarLocationPtr _location;
    public:
        static VariablePtr New(llvm::InstructionPtr inst, unsigned int totalCount) {
            return std::make_shared<Variable>(inst, totalCount);
        }
    private:
        explicit Variable(llvm::InstructionPtr& inst, unsigned int totalCount);
        void setLocation(VarLocationPtr loc);
    };

    class VarTable {
        std::map<llvm::InstructionPtr, MipsRegPtr> _table;
    };
}

#endif //VARTABLE_HPP
