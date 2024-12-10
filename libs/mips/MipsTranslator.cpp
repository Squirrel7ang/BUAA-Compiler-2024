//
// Created by tang on 12/6/24.
//

#include <memory>

#include "IR/Instructions.hpp"
#include "IR/Common.hpp"

#include "Insts.hpp"
#include "MipsImm.hpp"
#include "Regs.hpp"
#include "MipsTranslator.hpp"

namespace mips {
    MipsTranslator::MipsTranslator(llvm::ModulePtr& module)
        : _llvmModule(module),
        _varTable(VarTable::New(module)),
        _allocator(MipsRegAllocator::New(module, _varTable)) {

    }

    MipsTranslatorPtr MipsTranslator::New(llvm::ModulePtr module) {
        return std::make_shared<MipsTranslator>(module);
    }

    void MipsTranslator::allocReg() {
        _allocator->allocReg();
    }

    void MipsTranslator::translate() {
        allocReg();
        translate(_llvmModule);
    }

    void MipsTranslator::addMipsInst(MipsInstPtr mip) {
        _mipsInsts.push_back(mip);
    }

    void MipsTranslator::translate_allocStack(int bytes) {
        auto imm = MipsImm::New(bytes);
        auto rip = IInst::New(REG_SP, REG_SP, imm, MIID_ADDI);
        addMipsInst(rip);
    }

    void MipsTranslator::translate_freeStack(int bytes) {
        bytes = -bytes; // add in opposite direction
        auto imm = MipsImm::New(bytes);
        auto rip = IInst::New(REG_SP, REG_SP, imm, MIID_ADDI);
        addMipsInst(rip);
    }

    void MipsTranslator::translate(llvm::ModulePtr llvmModule) {
        auto&& beginIter = llvmModule->functionBegin();
        auto&& endIter = llvmModule->functionEnd();
        for (auto it = beginIter; it != endIter; ++it) {
            translate(*it);
        }
    }

    void MipsTranslator::translate(llvm::FunctionPtr llvmFunc) {
        // TODO: argument. might not needed
        // auto&& argBegin = llvmFunc->argumentBegin();
        // auto&& argEnd = llvmFunc->argumentEnd();
        // for (auto it = argBegin; it != argEnd; ++it) {
        //     translate(*it);
        // }

        // allocate stack space
        translate_allocStack(llvmFunc->spaceUse());

        // basicBlock
        auto&& blockBegin = llvmFunc->blockBegin();
        auto&& blockEnd = llvmFunc->blockEnd();
        for (auto it = blockBegin; it != blockEnd; ++it) {
            translate(*it);
        }
    }

    void MipsTranslator::translate(llvm::BasicBlockPtr llvmBlock) {
        auto&& beginIter = llvmBlock->instructionBegin();
        auto&& endIter = llvmBlock->instructionEnd();
        for (auto it = beginIter; it != endIter; ++it) {
            translate(*it);
        }
    }

    void MipsTranslator::translate(llvm::InstructionPtr llvmInst) {
        auto vty = llvmInst->getValueType();
        switch (llvmInst->getValueType()) {
            case llvm::UNARY_OPERATOR_T:
                translate(std::static_pointer_cast<llvm::UnaryOperator>(llvmInst));
                break;
            case llvm::BINARY_OPERATOR_T:
                translate(std::static_pointer_cast<llvm::BinaryOperator>(llvmInst));
                break;
            case llvm::COMPARE_INST_T:
                translate(std::static_pointer_cast<llvm::CompareInst>(llvmInst));
                break;
            case llvm::BRANCH_INST_T:
                translate(std::static_pointer_cast<llvm::BranchInst>(llvmInst));
                break;
            case llvm::JUMP_INST_T:
                translate(std::static_pointer_cast<llvm::JumpInst>(llvmInst));
                break;
            case llvm::LOAD_INST_T:
                translate(std::static_pointer_cast<llvm::LoadInst>(llvmInst));
                break;
            case llvm::STORE_INST_T:
                translate(std::static_pointer_cast<llvm::StoreInst>(llvmInst));
                break;
            case llvm::PUT_INST_T:
                translate(std::static_pointer_cast<llvm::PutInst>(llvmInst));
                break;
            case llvm::GETINT_INST_T:
                translate(std::static_pointer_cast<llvm::GetintInst>(llvmInst));
                break;
            case llvm::GETCHAR_INST_T:
                translate(std::static_pointer_cast<llvm::GetcharInst>(llvmInst));
                break;
            case llvm::GETELEPTR_INST_T:
                translate(std::static_pointer_cast<llvm::GetElePtrInst>(llvmInst));
                break;
            case llvm::CALL_INST_T:
                translate(std::static_pointer_cast<llvm::CallInst>(llvmInst));
                break;
            case llvm::ALLOCA_INST_T:
                translate(std::static_pointer_cast<llvm::AllocaInst>(llvmInst));
                break;
            case llvm::RETURN_INST_T:
                translate(std::static_pointer_cast<llvm::ReturnInst>(llvmInst));
                break;
            default: assert(0);
        }
    }

    void MipsTranslator::translate(llvm::ArgumentPtr llvmArgument) {
        // TODO: translate the argument?? seems no need;
    }

    void MipsTranslator::translate(llvm::UnaryOperatorPtr uop) {
    }

    void MipsTranslator::translate(llvm::BinaryOperatorPtr bop) {
    }

    void MipsTranslator::translate(llvm::CompareInstPtr cip) {
    }

    void MipsTranslator::translate(llvm::BranchInstPtr bip) {
    }

    void MipsTranslator::translate(llvm::JumpInstPtr jip) {
    }

    void MipsTranslator::translate(llvm::LoadInstPtr lip) {
    }

    void MipsTranslator::translate(llvm::StoreInstPtr sip) {
    }

    void MipsTranslator::translate(llvm::PutInstPtr pip) {
    }

    void MipsTranslator::translate(llvm::GetintInstPtr giip) {
    }

    void MipsTranslator::translate(llvm::GetcharInstPtr gcip) {
    }

    void MipsTranslator::translate(llvm::GetElePtrInstPtr gepip) {
    }

    void MipsTranslator::translate(llvm::CallInstPtr cip) {
    }

    void MipsTranslator::translate(llvm::AllocaInstPtr aip) {
    }

    void MipsTranslator::translate(llvm::ReturnInstPtr rip) {
    }
}
