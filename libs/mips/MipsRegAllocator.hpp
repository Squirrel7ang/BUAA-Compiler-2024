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
        StackPtr _curStack;
        std::map<llvm::FunctionPtr, StackPtr>& _stacks;
        SaveRegTablePtr _saveRegTable;
        TmpRegTablePtr _tmpRegTable;
        ConflictGraphPtr _graph;
    public:
        void allocReg();
        static MipsRegAllocatorPtr New(llvm::ModulePtr module, VarTablePtr, std::map<llvm::FunctionPtr, StackPtr>& stacks);
        explicit MipsRegAllocator(llvm::ModulePtr& module, VarTablePtr, std::map<llvm::FunctionPtr, StackPtr>& stacks);
    private:
        // allocate Save Register
        void allocSaveReg(llvm::ModulePtr& module);
        void allocSaveReg(llvm::FunctionPtr& func);

        // allocate sp for llvm::alloca inst
        void allocStackReg(const llvm::ModulePtr & module);
        void allocStackReg(llvm::InstructionPtr inst);

        // allocate temp register
        void allocTmpReg(llvm::ModulePtr &module);
        void allocTmpReg(llvm::FunctionPtr &module);
        void allocTmpReg(llvm::BasicBlockPtr module);
        void allocTmpReg(VariablePtr &var);

        // handle GetElePtr inst
        void handleGetElePtr(const llvm::ModulePtr & module);

        void handleGetElePtr(llvm::InstructionPtr inst);

        void handleGetElePtr(VariablePtr var);

    };
} // mips

#endif //MIPSREGALLOCATOR_HPP
