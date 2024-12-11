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
    public:
        std::set<VariablePtr> next;

    public:
        static VariablePtr New(llvm::InstructionPtr inst);
        void addConflictVar(VariablePtr var);
        VariablePtr removeConflictVar(VariablePtr vp);
        MipsRegPtr getTargetReg();
        explicit Variable(llvm::InstructionPtr& inst, unsigned int totalCount);
        bool hasLocation();
        void setLocation(VarLocationPtr loc);
        VarLocationPtr getLocation() { return _location; }
        void setOffset(unsigned int offset);
        int size();
        void countOneRef();
        bool reachRefCount();
    };

    class VarTable {
    private:
        std::map<llvm::InstructionPtr, VariablePtr> _vars;
    public:
        static VarTablePtr New(llvm::ModulePtr module);
        explicit VarTable(llvm::ModulePtr& module);

        VariablePtr findVar(llvm::InstructionPtr inst);
        std::map<llvm::InstructionPtr, VariablePtr>::iterator begin();
        std::map<llvm::InstructionPtr, VariablePtr>::iterator end();
        int size() { return _vars.size(); }
    private:
        void addVariable(llvm::InstructionPtr& inst, VariablePtr& var);
    };
}

#endif //VARTABLE_HPP
