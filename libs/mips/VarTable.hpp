//
// Created by tang on 11/26/24.
//

#ifndef VARTABLE_HPP
#define VARTABLE_HPP

#include <map>
#include <set>

#include "MipsCommon.hpp"
#include "IR/Common.hpp"
#include "IR/Instructions.hpp"

namespace mips {
    class Variable {
        // llvm
        llvm::InstructionPtr _llvmInst;

        // reference count
        const unsigned int _totalCounts;
        unsigned int _refCounts;

        // storage
        VarLocationPtr _location;

        // Conflict Graph
        std::vector<VariablePtr> next;
    public:
        static VariablePtr New(llvm::InstructionPtr inst);
        void addConflictVar(VariablePtr var);
    private:
        explicit Variable(llvm::InstructionPtr& inst, unsigned int totalCount);
        void setLocation(VarLocationPtr loc);
    };

    class VarTable {
    private:
        std::map<llvm::InstructionPtr, VariablePtr> _vars;
    public:
        VariablePtr findVar(llvm::InstructionPtr inst);
        static VarTablePtr New(llvm::ModulePtr module);

    private:
        explicit VarTable(llvm::ModulePtr& module);
        void addVariable(llvm::InstructionPtr& inst, VariablePtr& var);
    };
}

#endif //VARTABLE_HPP
