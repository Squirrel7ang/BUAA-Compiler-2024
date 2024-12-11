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

#include "MipsData.hpp"
#include "MipsLabel.hpp"
#include "StackSlot.hpp"

namespace mips {
    MipsTranslator::MipsTranslator(llvm::ModulePtr& module)
        : _llvmModule(module),
        _varTable(VarTable::New(module)),
        _dataTable(DataTable::New(module)),
        _labelTable(LabelTable::New(module)),
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
        auto&& globalVarBegin = llvmModule->globalVarBegin();
        auto&& globalVarEnd = llvmModule->globalVarEnd();
        for (auto it = globalVarBegin; it != globalVarEnd; ++it) {
            translate(*it);
        }

        auto&& stringBegin = llvmModule->stringBegin();
        auto&& stringEnd = llvmModule->stringEnd();
        for (auto it = stringBegin; it != stringEnd; ++it) {
            translate(*it);
        }

        auto&& funcBegin = llvmModule->functionBegin();
        auto&& funcEnd = llvmModule->functionEnd();
        for (auto it = funcBegin; it != funcEnd; ++it) {
            translate(*it);
        }
    }

    void MipsTranslator::translate(llvm::GlobalVariablePtr llvmGlobalVar) {
        // auto mipsData = MipsData::New(llvmGlobalVar);
        // addGlobalData(llvmGlobalVar, mipsData);
        // _mipsDatas.push_back(mipsData);
    }

    void MipsTranslator::translate(llvm::GlobalStringPtr llvmString) {
        // auto mipsData = MipsData::New(llvmString);
        // addGlobalData(llvmGlobalVar, mipsData);
        // _mipsDatas.push_back(mipsData);
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
        // TODO: might be a problem, because op0 might be a llvm::Constant.
        auto var = _varTable->findVar(uop);

        auto usee0 = uop->getUsee(0);

        auto reg0 = readToReg(usee0, true);
        MipsRegPtr targetReg = var->getTargetReg();

        switch (uop->getUnaryOpId()) {
            case llvm::UOID_ZEXT:
                // Do nothing
                break;
            case llvm::UOID_TRUNC: {
                auto inst = IInst::New(reg0, targetReg, IMM_TRUNC_BYTE, MIID_ANDI);
                addMipsInst(inst);
                break;
            }
            default: assert(0);
        }

        writeBackReg(targetReg, var);
    }

    void MipsTranslator::translate(llvm::BinaryOperatorPtr bop) {
        auto var = _varTable->findVar(bop);

        auto&& usee0 = bop->getUsee(0);
        auto&& usee1 = bop->getUsee(1);

        auto reg0 = readToReg(usee0, true);
        auto reg1 = readToReg(usee1, false);
        auto targetReg = var->getTargetReg();

        MipsInstPtr inst0, inst1;
        switch (bop->getBinaryOpId()) {
            case llvm::BOID_ADD:
                inst0 = RInst::New(reg0, reg1, targetReg, MIID_ADD);
                addMipsInst(inst0);
                break;
            case llvm::BOID_SUB:
                inst0 = RInst::New(reg0, reg1, targetReg, MIID_SUB);
                addMipsInst(inst0);
                break;
            case llvm::BOID_MUL:
                inst0 = RInst::New(reg0, reg1, REG_ZERO, MIID_MULT);
                inst1 = RInst::New(REG_ZERO, REG_ZERO, targetReg, MIID_MFLO);
                addMipsInst(inst0);
                addMipsInst(inst1);
                break;
            case llvm::BOID_DIV:
                inst0 = RInst::New(reg0, reg1, REG_ZERO, MIID_DIV);
                inst1 = RInst::New(REG_ZERO, REG_ZERO, targetReg, MIID_MFLO);
                addMipsInst(inst0);
                addMipsInst(inst1);
                break;
            case llvm::BOID_REM:
                inst0 = RInst::New(reg0, reg1, REG_ZERO, MIID_DIV);
                inst1 = RInst::New(REG_ZERO, REG_ZERO, targetReg, MIID_MFHI);
                addMipsInst(inst0);
                addMipsInst(inst1);
                break;
            default: assert(0);
        }
        writeBackReg(targetReg, var);
    }

    void MipsTranslator::translate(llvm::CompareInstPtr cip) {
        auto var = _varTable->findVar(cip);
        auto&& usee0 = cip->getUsee(0);
        auto&& usee1 = cip->getUsee(1);

        auto reg0 = readToReg(usee0, true);
        auto reg1 = readToReg(usee1, false);
        auto targetReg = var->getTargetReg();

        MipsInstPtr inst;
        switch (cip->getCompareId()) {
            case llvm::CIID_EQ:
                inst = RInst::New(reg0, reg1, targetReg, MIID_SEQ);
                addMipsInst(inst);
                break;
            case llvm::CIID_NE:
                inst = RInst::New(reg0, reg1, targetReg, MIID_SNE);
                addMipsInst(inst);
                break;
            case llvm::CIID_SLE:
                inst = RInst::New(reg0, reg1, targetReg, MIID_SLE);
                addMipsInst(inst);
                break;
            case llvm::CIID_SLT:
                inst = RInst::New(reg0, reg1, targetReg, MIID_SLT);
                addMipsInst(inst);
                break;
            case llvm::CIID_SGE:
                inst = RInst::New(reg0, reg1, targetReg, MIID_SGE);
                addMipsInst(inst);
                break;
            case llvm::CIID_SGT:
                inst = RInst::New(reg0, reg1, targetReg, MIID_SGT);
                addMipsInst(inst);
                break;
            default: assert(0);
        }
        writeBackReg(targetReg, var);
    }

    void MipsTranslator::translate(llvm::BranchInstPtr bip) {
        auto&& usee0 = bip->getUsee(0);
        auto&& usee1 = bip->getUsee(1);
        auto&& usee2 = bip->getUsee(2);

        auto reg0 = readToReg(usee0, true);
        auto label0 = readToImm(usee1);
        auto label1 = readToImm(usee2);

        auto inst0 = IInst::New(REG_ZERO, reg0, label1, MIID_BNE);
        auto inst1 = JInst::New(label0, MIID_J);
        addMipsInst(inst0);
        addMipsInst(inst1);
    }

    void MipsTranslator::translate(llvm::JumpInstPtr jip) {
        auto&& usee0 = jip->getUsee(0);
        auto label0 = readToImm(usee0);

        auto inst0 = JInst::New(label0, MIID_J);
        addMipsInst(inst0);
    }

    void MipsTranslator::translate(llvm::LoadInstPtr lip) {
        assert(lip->getType()->isPointer());
        auto var = _varTable->findVar(lip);
        auto usee0 = lip->getUsee(0);

        auto targetReg = var->getTargetReg();
        auto&& llvmType = std::static_pointer_cast<llvm::PointerType>(lip->getType());
        unsigned int size = llvmType->getBasicType()->getSize();
        assert(size == 1 || size == 4);

        MipsInstPtr inst0, inst1;
        if (usee0->isInst()) {
            auto inst = std::static_pointer_cast<llvm::Instruction>(usee0);
            auto prevVar = _varTable->findVar(inst);
            auto reg = readToReg(inst, true);
            unsigned int offset = prevVar->getLocation()->getInitOffset();
            auto imm = MipsImm::New(offset);
            if (size == 1) {
                inst0 = IInst::New(reg, targetReg, imm, MIID_LB);
            }
            else {
                inst0 = IInst::New(reg, targetReg, imm, MIID_LW);
            }
            addMipsInst(inst0);
        }
        else if (usee0->is(llvm::GLOBAL_STRING_T) ||
                 usee0->is(llvm::GLOBAL_VARIABLE_T)) {
            auto globalVal = std::static_pointer_cast<llvm::GlobalValue>(usee0);
            auto data = _dataTable->findData(globalVal);

            inst0 = IInst::New(REG_ZERO, REG_K0, data, MIID_LA);
            addMipsInst(inst0);
            if (size == 1) {
                inst1 = IInst::New(REG_K0, targetReg, IMM_ZERO, MIID_LB);
            }
            else {
                inst1 = IInst::New(REG_K0, targetReg, IMM_ZERO, MIID_LW);
            }
            addMipsInst(inst1);
        }
        else {
            assert(0);
        }
        writeBackReg(targetReg, var);
    }

    void MipsTranslator::translate(llvm::StoreInstPtr sip) {
        auto useVal = sip->getUsee(0);
        auto useAddr = sip->getUsee(1);
        // TODO: what if useAddr is stored in register?

        unsigned int size = useVal->getType()->getSize();
        assert(size == 1 || size == 4);
        if (useAddr->is(llvm::GLOBAL_STRING_T) ||
            useAddr->is(llvm::GLOBAL_STRING_T)) {
            auto valReg = readToReg(useVal, true);
            auto offset = MipsImm::New(0);
            auto addrImm = readToImm(useAddr);
            addMipsInst(IInst::NewLa(REG_K1, addrImm));
            if (size == 1) {
                addMipsInst(IInst::New(REG_K1, valReg, offset, MIID_SB));
            }
            else {
                addMipsInst(IInst::New(REG_K1, valReg, offset, MIID_SW));
            }
        }
        else if (useAddr->isInst()) {
            auto&& prevAddrInst = std::static_pointer_cast<llvm::Instruction>(useAddr);
            auto&& prevAddrVar = _varTable->findVar(prevAddrInst);
            auto&& _offset = prevAddrVar->getLocation()->getInitOffset();

            auto offset = MipsImm::New(_offset);
            auto valReg = readToReg(useVal, true);
            auto addrReg = readToReg(useAddr, false);
            if (size == 1) {
                addMipsInst(IInst::New(addrReg, valReg, offset, MIID_SB));
            }
            else {
                addMipsInst(IInst::New(addrReg, valReg, offset, MIID_SW));
            }
        }
    }

    void MipsTranslator::translate(llvm::PutInstPtr pip) {
        auto usee0 = pip->getUsee(0);

        MipsRegPtr reg;
        MipsImmPtr imm;
        switch (pip->getPutID()) {
            case llvm::PIID_CH:
                reg = readToReg(usee0, true);
                imm = MipsImm::New(11);
                addMipsInst(RInst::NewMove(reg, REG_A0));
                addMipsInst(IInst::NewLi(REG_V0, imm));
                addMipsInst(RInst::NewSyscall());
                break;
            case llvm::PIID_INT:
                reg = readToReg(usee0, true);
                imm = MipsImm::New(1);
                addMipsInst(RInst::NewMove(reg, REG_A0));
                addMipsInst(IInst::NewLi(REG_V0, imm));
                addMipsInst(RInst::NewSyscall());
                break;
            case llvm::PIID_STR:
                MipsImmPtr strAddr = readToImm(usee0);
                imm = MipsImm::New(4);
                addMipsInst(IInst::NewLa(REG_A0, strAddr));;
                addMipsInst(IInst::NewLi(REG_V0, imm));
                addMipsInst(RInst::NewSyscall());
                break;
            default: assert(0);
        }
    }

    void MipsTranslator::translate(llvm::GetintInstPtr giip) {
        auto&& var = _varTable->findVar(giip);

        auto targetReg = var->getTargetReg();

        auto imm = MipsImm::New(5);
        addMipsInst(IInst::NewLi(REG_V0, imm));
        addMipsInst(RInst::NewSyscall());
        addMipsInst(RInst::NewMove(REG_V0, targetReg));

        writeBackReg(targetReg, var);
    }

    void MipsTranslator::translate(llvm::GetcharInstPtr gcip) {
        auto&& var = _varTable->findVar(gcip);

        auto targetReg = var->getTargetReg();

        auto imm = MipsImm::New(12);
        addMipsInst(IInst::NewLi(REG_V0, imm));
        addMipsInst(RInst::NewSyscall());
        addMipsInst(RInst::NewMove(REG_V0, targetReg));

        writeBackReg(targetReg, var);
    }

    void MipsTranslator::translate(llvm::GetElePtrInstPtr gepip) {
    }

    void MipsTranslator::translate(llvm::CallInstPtr cip) {
    }

    void MipsTranslator::translate(llvm::AllocaInstPtr aip) {
        // Do Nothing
    }

    void MipsTranslator::translate(llvm::ReturnInstPtr rip) {
        auto usee = rip->getUsee(0);

        auto reg = readToReg(usee, true);

        addMipsInst(RInst::NewMove(reg, REG_V0));
        addMipsInst(RInst::NewJr());
    }

    MipsRegPtr MipsTranslator::readToReg(llvm::ValuePtr vp, bool firstOp) {
        if (vp->isInst()) {
            return readToReg(std::static_pointer_cast<llvm::Instruction>(vp), firstOp);
        }
        else if (vp->is(llvm::GLOBAL_VARIABLE_T)) {
            return readToReg(std::static_pointer_cast<llvm::GlobalVariable>(vp), firstOp);
        }
        else if (vp->is(llvm::GLOBAL_STRING_T)) {
            return readToReg(std::static_pointer_cast<llvm::GlobalString>(vp), firstOp);
        }
        else if (vp->is(llvm::CONSTANT_DATA_T)) {
            return readToReg(std::static_pointer_cast<llvm::ConstantData>(vp), firstOp);
        }
        else {
            assert(0);
        }
    }

    MipsRegPtr MipsTranslator::readToReg(llvm::InstructionPtr inst, bool firstOp) {
        auto var = _varTable->findVar(inst);

        if (var->getLocation()->is(VLID_REG)) {
            return std::static_pointer_cast<MipsReg>(var->getLocation());
        }
        else {
            MipsRegPtr reg;
            if (firstOp) reg = REG_K0;
            else reg = REG_K1;

            auto stackLoc = std::static_pointer_cast<StackSlot>(var->getLocation());
            IInstPtr iinst;
            MipsImmPtr imm = MipsImm::New(stackLoc->getOffset());
            if (stackLoc->getSize() == 1) {
                iinst = IInst::New(REG_SP, reg, imm, MIID_LB);
            }
            else if (stackLoc ->getSize() == 4) {
                iinst = IInst::New(REG_SP, reg, imm, MIID_LW);
            }

            addMipsInst(iinst);
            return reg;
        }
    }

    MipsRegPtr MipsTranslator::readToReg(llvm::GlobalVariablePtr var, bool firstOp) {
        // TODO: no need to load out
        assert(0);
        auto offset = _dataTable->findData(var);
        MipsRegPtr reg;
        if (firstOp)  reg = REG_K0;
        else reg = REG_K1;
        if (offset->isWord()) {
            auto&& inst = IInst::New(REG_ZERO, reg, offset, MIID_LW);
            addMipsInst(inst);
        }
        else if (offset->isByte()) {
            auto&& inst = IInst::New(REG_ZERO, reg, offset, MIID_LB);
            addMipsInst(inst);
        }
        else
            assert(0);
        return reg;
    }

    MipsRegPtr MipsTranslator::readToReg(llvm::GlobalStringPtr str, bool firstOp) {
        // TODO: no need to load out
        assert(0);
        auto offset = _dataTable->findData(str);
        MipsRegPtr reg;
        if (firstOp)  reg = REG_K0;
        else reg = REG_K1;

        assert(offset->isString());
        auto&& inst = IInst::New(REG_ZERO, reg, offset, MIID_LB);
        addMipsInst(inst);
        return reg;
    }

    MipsRegPtr MipsTranslator::readToReg(llvm::ConstantDataPtr data, bool firstOp) {
        assert(data->size() == 1);

        MipsRegPtr reg;
        if (firstOp) reg = REG_K0;
        else reg = REG_K1;

        auto imm = MipsImm::New(data->getVal(0));
        auto inst = IInst::New(REG_ZERO, reg, imm, MIID_LI);
        addMipsInst(inst);
        return reg;
    }

    MipsImmPtr MipsTranslator::readToImm(llvm::ValuePtr vp) {
        if (vp->is(llvm::CONSTANT_DATA_T)) {
            return readToImm(std::static_pointer_cast<llvm::ConstantData>(vp));
        }
        else if (vp->is(llvm::BASIC_BLOCK_T)) {
            return readToImm(std::static_pointer_cast<llvm::BasicBlock>(vp));
        }
        else if (vp->is(llvm::GLOBAL_VARIABLE_T)) {
            return readToImm(std::static_pointer_cast<llvm::GlobalVariable>(vp));
        }
        else if (vp->is(llvm::GLOBAL_STRING_T)) {
            return readToImm(std::static_pointer_cast<llvm::GlobalString>(vp));
        }
        else {
            assert(0);
        }
    }

    MipsImmPtr MipsTranslator::readToImm(llvm::ConstantDataPtr vp) {
        assert(vp->size() == 1);
        MipsImmPtr&& imm = MipsImm::New(vp->getVal(0));
        return imm;
    }

    MipsImmPtr MipsTranslator::readToImm(llvm::BasicBlockPtr vp) {
        auto&& label = _labelTable->findLabel(vp);
        return label;
    }

    MipsImmPtr MipsTranslator::readToImm(llvm::GlobalVariablePtr vp) {
        auto&& data = _dataTable->findData(vp);
        return data;
    }

    MipsImmPtr MipsTranslator::readToImm(llvm::GlobalStringPtr vp) {
        auto&& data = _dataTable->findData(vp);
        return data;
    }

    void MipsTranslator::writeBackReg(MipsRegPtr & reg, VariablePtr & var) {
        // TODO
    }
}
