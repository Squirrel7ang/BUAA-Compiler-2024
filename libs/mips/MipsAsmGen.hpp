//
// Created by tang on 11/26/24.
//

#ifndef MIPSASM_HPP
#define MIPSASM_HPP

#include "IR/Common.hpp"
#include "MipsCommon.hpp"

namespace mips {
    /**
     * This is the top module of MIPS asm generator
     */
    class MipsAsm {
        // TODO: global variable definition
        // llvm
        llvm::ModulePtr _llvmModule;

        std::vector<MipsInstPtr> _mipsInsts;
        VarTablePtr _varTable;
    public:
        void addInst(MipsInstPtr);
        void translate();
        void print(std::ostream& out);
    private:
        void translate(llvm::ModulePtr llvmModule);
        void translate(llvm::FunctionPtr llvmFunc);
        void translate(llvm::BasicBlockPtr llvmBlock);
        void translate(llvm::InstructionPtr llvmInst);
        void translate(llvm::ArgumentPtr llvmArgument);

        // Instructions
        void translate(llvm::UnaryOperatorPtr uop);
        void translate(llvm::BinaryOperatorPtr bop);
        void translate(llvm::CompareInstPtr cip);
        void translate(llvm::BranchInstPtr bip);
        void translate(llvm::JumpInstPtr jip);
        void translate(llvm::LoadInstPtr lip);
        void translate(llvm::StoreInstPtr sip);
        void translate(llvm::PutInstPtr pip);
        void translate(llvm::GetintInstPtr giip);
        void translate(llvm::GetcharInstPtr gcip);
        void translate(llvm::GetElePtrInstPtr gepip);
        void translate(llvm::CallInstPtr cip);
        void translate(llvm::AllocaInstPtr aip);
        void translate(llvm::ReturnInstPtr rip);
    };

}

#endif //MIPSASM_HPP
