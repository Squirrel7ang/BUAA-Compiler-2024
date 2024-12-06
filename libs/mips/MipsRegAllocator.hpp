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
        ConflictGraphPtr _graph;
        VarTable _varTable;
        SaveRegTable _saveRegTable;
        TmpRegTable _tmpRegTable;
    public:
        void allocReg();
    private:
        // allocate Save Register
        void allocSaveReg(llvm::ModulePtr module);
        void allocSaveReg(llvm::FunctionPtr func);

        // allocate temp register
        void allocTmpReg(llvm::ModulePtr module);
        void allocTmpReg(llvm::FunctionPtr module);
        void allocTmpReg(llvm::BasicBlockPtr module);

        // universal allocation of register for variables.
        void allocReg(llvm::InstructionPtr inst, MipsRegPtr reg);
    };
} // mips

#endif //MIPSREGALLOCATOR_HPP
