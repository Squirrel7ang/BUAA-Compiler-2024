//
// Created by tang on 12/6/24.
//

#ifndef MIPSTRANSLATOR_HPP
#define MIPSTRANSLATOR_HPP

#include "MipsCommon.hpp"
#include "MipsRegAllocator.hpp"
#include "IR/Module.hpp"

namespace mips {
    class MipsTranslator {
    private:
        llvm::ModulePtr _llvmModule;
        VarTablePtr _varTable;
        MipsRegAllocatorPtr _allocator;
        std::vector<MipsInstPtr> _mipsInsts; // the generated asm in .text field
    public:
        static MipsTranslatorPtr New(llvm::ModulePtr module);
        void allocReg();
        void translate();
    private:
        explicit MipsTranslator(llvm::ModulePtr& module);
        void addMipsInst(MipsInstPtr mip);
        void translate_allocStack(int bytes);
        void translate_freeStack(int bytes);

        // translate module
        void translate(llvm::ModulePtr llvmModule);
        void translate(llvm::FunctionPtr llvmFunc);
        void translate(llvm::BasicBlockPtr llvmBlock);
        void translate(llvm::InstructionPtr llvmInst);
        void translate(llvm::ArgumentPtr llvmArgument);

        // translate instruction
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

#endif //MIPSTRANSLATOR_HPP
