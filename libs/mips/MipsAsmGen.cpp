//
// Created by tang on 11/26/24.
//

#include "MipsAsmGen.hpp"

#include "IR/Instructions.hpp"
#include "IR/Module.hpp"

namespace mips {
    void MipsAsm::addInst(MipsInstPtr mipsInst) {
    }

    void MipsAsm::translate() {
        translate(_llvmModule);
    }

    void MipsAsm::print(std::ostream &out) {
    }

    void MipsAsm::translate(llvm::ModulePtr llvmModule) {
        auto&& beginIter = llvmModule->functionBegin();
        auto&& endIter = llvmModule->functionEnd();
        for (auto it = beginIter; it != endIter; ++it) {
            translate(*it);
        }
    }

    void MipsAsm::translate(llvm::FunctionPtr llvmFunc) {
        auto&& blockBegin = llvmFunc->blockBegin();
        auto&& blockEnd = llvmFunc->blockEnd();
        auto&& argBegin = llvmFunc->argumentBegin();
        auto&& argEnd = llvmFunc->argumentEnd();
        for (auto it = argBegin; it != argEnd; ++it) {
            translate(*it);
        }
        for (auto it = blockBegin; it != blockEnd; ++it) {
            translate(*it);
        }
    }

    void MipsAsm::translate(llvm::BasicBlockPtr llvmBlock) {
        auto&& beginIter = llvmBlock->instructionBegin();
        auto&& endIter = llvmBlock->instructionEnd();
        for (auto it = beginIter; it != endIter; ++it) {
            translate(*it);
        }
    }

    void MipsAsm::translate(llvm::InstructionPtr llvmInst) {
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

    void MipsAsm::translate(llvm::ArgumentPtr llvmArgument) {
    }

    void MipsAsm::translate(llvm::UnaryOperatorPtr uop) {
    }

    void MipsAsm::translate(llvm::BinaryOperatorPtr bop) {
    }

    void MipsAsm::translate(llvm::CompareInstPtr cip) {
    }

    void MipsAsm::translate(llvm::BranchInstPtr bip) {
    }

    void MipsAsm::translate(llvm::JumpInstPtr jip) {
    }

    void MipsAsm::translate(llvm::LoadInstPtr lip) {
    }

    void MipsAsm::translate(llvm::StoreInstPtr sip) {
    }

    void MipsAsm::translate(llvm::PutInstPtr pip) {
    }

    void MipsAsm::translate(llvm::GetintInstPtr giip) {
    }

    void MipsAsm::translate(llvm::GetcharInstPtr gcip) {
    }

    void MipsAsm::translate(llvm::GetElePtrInstPtr gepip) {
    }

    void MipsAsm::translate(llvm::CallInstPtr cip) {
    }

    void MipsAsm::translate(llvm::AllocaInstPtr aip) {
    }

    void MipsAsm::translate(llvm::ReturnInstPtr rip) {
    }
}
