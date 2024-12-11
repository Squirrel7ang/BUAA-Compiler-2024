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
        DataTablePtr _dataTable;
        LabelTablePtr _labelTable;
        MipsRegAllocatorPtr _allocator;
        std::vector<MipsInstPtr> _mipsInsts; // the generated asm in .text field
        // std::vector<MipsDataPtr> _mipsDatas;
    public:
        static MipsTranslatorPtr New(llvm::ModulePtr module);
        explicit MipsTranslator(llvm::ModulePtr& module);
        void allocReg();
        void translate();
    private:
        void addMipsInst(MipsInstPtr mip);

        void translate_allocStack(int bytes);
        void translate_freeStack(int bytes);

        // translate module
        void translate(llvm::ModulePtr llvmModule);
        void translate(llvm::GlobalStringPtr llvmString);
        void translate(llvm::GlobalVariablePtr llvmGlobalVar);
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

        MipsRegPtr readToReg(llvm::ValuePtr vp, bool firstOp);
        MipsRegPtr readToReg(llvm::InstructionPtr inst, bool firstOp);
        MipsRegPtr readToReg(llvm::GlobalVariablePtr var, bool firstOp);
        MipsRegPtr readToReg(llvm::GlobalStringPtr str, bool firstOp);
        MipsRegPtr readToReg(llvm::ConstantDataPtr data, bool firstOp);

        MipsImmPtr readToImm(llvm::ValuePtr vp);
        MipsImmPtr readToImm(llvm::ConstantDataPtr vp);
        MipsImmPtr readToImm(llvm::BasicBlockPtr vp);
        MipsImmPtr readToImm(llvm::GlobalVariablePtr vp);
        MipsImmPtr readToImm(llvm::GlobalStringPtr vp);

        void writeBackReg(MipsRegPtr & reg, VariablePtr & var);
    };
}

#endif //MIPSTRANSLATOR_HPP
