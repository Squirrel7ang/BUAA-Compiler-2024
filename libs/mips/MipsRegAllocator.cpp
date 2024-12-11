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
        allocStackReg(_llvmModule);
        // TODO: handle geteleptr inst. Such inst should always be a none-cross-block variable
        allocTmpReg(_llvmModule);
        handleGetElePtr(_llvmModule);
    }

    MipsRegAllocatorPtr MipsRegAllocator::New(llvm::ModulePtr module, VarTablePtr varTable) {
        return std::make_shared<MipsRegAllocator>(module, varTable);
    }

    MipsRegAllocator::MipsRegAllocator(llvm::ModulePtr& module, VarTablePtr varTable)
        : _llvmModule(module),
        _varTable(varTable),
        _stack(Stack::New()),
        _saveRegTable(SaveRegTable::New()),
        _tmpRegTable(TmpRegTable::New()),
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

    void MipsRegAllocator::allocStackReg(const llvm::ModulePtr &module) {
        auto&& funcBegin = module->functionBegin();
        auto&& funcEnd = module->functionEnd();
        for (auto f = funcBegin; f != funcEnd; ++f) {
            auto&& blockBegin = (*f)->blockBegin();
            auto&& blockEnd = (*f)->blockEnd();
            for (auto b = blockBegin; b != blockEnd; ++b) {
                auto&& instBegin = (*b)->instructionBegin();
                auto&& instEnd = (*b)->instructionEnd();
                for (auto i = instBegin; i != instEnd; ++i) {
                    if ((*i)->getValueType() == llvm::ALLOCA_INST_T) {
                        allocStackReg(*i);
                    }
                }
            }
        }
    }

    void MipsRegAllocator::allocStackReg(llvm::InstructionPtr inst) {
        auto var = _varTable->findVar(inst);
        assert(!var->hasLocation());

        auto ty = std::dynamic_pointer_cast<llvm::PointerType>(inst->getType());
        unsigned int size = ty->getSize();

        auto slot = _stack->allocateSlot(size);
        MipsRegPtr spWithOffset = MipsReg::New(REG_SP_NUM, slot->getOffset());

        var->setLocation(spWithOffset);
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
            if (!(*it)->getType()->isVoidTy()) {
                auto var = _varTable->findVar(*it);
                if (!var->hasLocation() && var != nullptr) {
                    allocTmpReg(var);
                }
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

    void MipsRegAllocator::handleGetElePtr(const llvm::ModulePtr &module) {
        auto&& funcBegin = module->functionBegin();
        auto&& funcEnd = module->functionEnd();
        for (auto f = funcBegin; f != funcEnd; ++f) {
            auto&& blockBegin = (*f)->blockBegin();
            auto&& blockEnd = (*f)->blockEnd();
            for (auto b = blockBegin; b != blockEnd; ++b) {
                auto&& instBegin = (*b)->instructionBegin();
                auto&& instEnd = (*b)->instructionEnd();
                for (auto i = instBegin; i != instEnd; ++i) {
                    if ((*i)->getValueType() == llvm::GETELEPTR_INST_T) {
                        handleGetElePtr(*i);
                    }
                }
            }
        }
    }

    void MipsRegAllocator::handleGetElePtr(llvm::InstructionPtr inst) {
        auto _usee = inst->getUsee(0);
        if (_usee->is(llvm::GLOBAL_VARIABLE_T)) {
            return;
        }
        assert(_usee->isInst());
        auto prevInst = std::dynamic_pointer_cast<llvm::Instruction>(_usee);
        assert(prevInst->getType()->isPointer());

        auto curVar = _varTable->findVar(inst);
        auto prevVar = _varTable->findVar(prevInst);
        curVar->setOffset(prevVar->getLocation()->getInitOffset());
    }

} // mips