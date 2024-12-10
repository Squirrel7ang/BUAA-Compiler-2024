//
// Created by tang on 12/6/24.
//

#include "MipsRegAllocator.hpp"

#include "Regs.hpp"
#include "StackSlot.hpp"
#include "IR/Module.hpp"

namespace mips {
    void MipsRegAllocator::allocReg() {
        allocSaveReg(_llvmModule);
        allocTmpReg(_llvmModule);
    }

    MipsRegAllocatorPtr MipsRegAllocator::New(llvm::ModulePtr module, VarTablePtr varTable) {
        return std::make_shared<MipsRegAllocator>(module, varTable);
    }

    MipsRegAllocator::MipsRegAllocator(llvm::ModulePtr& module, VarTablePtr varTable)
        : _llvmModule(module),
        _varTable(varTable),
        _stack(Stack::New()),
        _saveRegTable(),
        _tmpRegTable(),
        _graph(ConflictGraph::New(_varTable, _saveRegTable, _stack)) {
    }

    void MipsRegAllocator::allocSaveReg(llvm::ModulePtr& module) {
        auto&& begin = module->functionBegin();
        auto&& end = module->functionEnd();
        for (auto it = begin; it != end; ++it) {
            allocSaveReg(*it);
        }
    }

    void MipsRegAllocator::allocSaveReg(llvm::FunctionPtr& func) {
        auto&& begin = func->blockBegin();
        auto&& end = func->blockEnd();

        // the edge of the conflict graph should be calculated
        // during dataflow analysis.
        _graph->insertEdges(func->getConflictVars());
        _graph->dyeEveryNode();
    }

    void MipsRegAllocator::allocTmpReg(llvm::ModulePtr &module) {
        auto&& begin = module->functionBegin();
        auto&& end = module->functionEnd();
        for (auto it = begin; it != end; ++it) {
            allocTmpReg(*it);
        }
    }

    void MipsRegAllocator::allocTmpReg(llvm::FunctionPtr &func) {
        auto&& begin = func->blockBegin();
        auto&& end = func->blockEnd();
        for (auto it = begin; it != end; ++it) {
            allocTmpReg(*it);
        }
    }

    void MipsRegAllocator::allocTmpReg(llvm::BasicBlockPtr block) {
        auto&& begin = block->instructionBegin();
        auto&& end = block->instructionEnd();
        for (auto it = begin; it != end; ++it) {
            auto var = _varTable->findVar(*it);
            if (!var->hasLocation() && var != nullptr) {
                allocTmpReg(var);
            }
        }
    }

    void MipsRegAllocator::allocTmpReg(VariablePtr& var) {
        if (_tmpRegTable->isFree()) {
            auto reg = _tmpRegTable->allocateReg();
            var->setLocation(reg);
            _tmpRegTable->occupyReg(var, reg);
        }
        else {
            auto slot = _stack->allocateSlot(var->size());
            var->setLocation(slot);
        }
    }
} // mips