//
// Created by tang on 12/6/24.
//

#ifndef MIPSREGALLOCATOR_HPP
#define MIPSREGALLOCATOR_HPP

#include "ConflictGraph.hpp"
#include "TmpRegTable.hpp"
#include "SaveRegTable.hpp"
#include "VarTable.hpp"

namespace mips {
    class MipsRegAllocator {
    private:
        llvm::ModulePtr _llvmModule;
        VarTablePtr _varTable;
        ConflictGraphPtr _graph;
        SaveRegTablePtr _saveRegTable;
        TmpRegTablePtr _tmpRegTable;
    public:
        void allocReg();
        static MipsRegAllocatorPtr New(llvm::ModulePtr module);
    private:
        explicit MipsRegAllocator(llvm::ModulePtr& module);
        // allocate Save Register
        void allocSaveReg(llvm::ModulePtr& module);
        void allocSaveReg(llvm::FunctionPtr& func);

        // allocate temp register
        void allocTmpReg(llvm::ModulePtr &module);
        void allocTmpReg(llvm::FunctionPtr &module);
        void allocTmpReg(llvm::BasicBlockPtr module);

        // universal allocation of register for variables.
        void allocReg(VariablePtr& var, MipsRegPtr& reg);
    };
} // mips

#endif //MIPSREGALLOCATOR_HPP
