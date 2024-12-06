//
// Created by tang on 12/6/24.
//

#include "MipsRegAllocator.hpp"

#include "Regs.hpp"
#include "IR/Module.hpp"

namespace mips {
    void MipsRegAllocator::allocReg() {
        allocSaveReg(_llvmModule);
        allocTmpReg(_llvmModule);
    }

    MipsRegAllocatorPtr MipsRegAllocator::New(llvm::ModulePtr module) {
        return std::make_shared<MipsRegAllocator>(module);
    }

    MipsRegAllocator::MipsRegAllocator(llvm::ModulePtr& module)
        : _llvmModule(module),
        _varTable(VarTable::New(module)),
        _graph(ConflictGraph::New(_varTable)),
        _saveRegTable(),
        _tmpRegTable() {

    }

    void MipsRegAllocator::allocSaveReg(llvm::ModulePtr& module) {
        auto&& begin = module->functionBegin();
        auto&& end = module->functionEnd();
        for (auto it = begin; it != end; it++) {
            allocSaveReg(*it);
        }
    }

    void MipsRegAllocator::allocSaveReg(llvm::FunctionPtr& func) {
        auto&& begin = func->blockBegin();
        auto&& end = func->blockEnd();

        // the edge of the conflict graph should be calculated
        // during dataflow analysis.
        _graph->insertEdges(func->getConflictVars());

        // TODO: color it!!
    }

    void MipsRegAllocator::allocTmpReg(llvm::ModulePtr &module) {
        auto&& begin = module->functionBegin();
        auto&& end = module->functionEnd();
        for (auto it = begin; it != end; it++) {

        }
    }

    void MipsRegAllocator::allocTmpReg(llvm::FunctionPtr &func) {
        auto&& begin = func->blockBegin();
        auto&& end = func->blockEnd();
        for (auto it = begin; it != end; it++) {

        }
    }

    void MipsRegAllocator::allocTmpReg(llvm::BasicBlockPtr block) {
        auto&& begin = block->instructionBegin();
        auto&& end = block->instructionEnd();
        for (auto it = begin; it != end; it++) {

        }
    }

    void MipsRegAllocator::allocReg(VariablePtr& var, MipsRegPtr& reg) {
        if (reg->getType() == MRT_SAVE) {
            _saveRegTable->insert(var, reg);
        }
        else if (reg->getType() == MRT_TMP) {
            _tmpRegTable->insert(var, reg);
        }
    }
} // mips