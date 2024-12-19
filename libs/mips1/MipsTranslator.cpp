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

namespace mips {
    MipsTranslator::MipsTranslator(llvm::ModulePtr& module)
        : _llvmModule(module),
        _varTable(VarTable::New(module)),
        _dataTable(DataTable::New(module)),
        _stacks(),
        _labelTable(LabelTable::New(module)),
        _allocator(MipsRegAllocator::New(module, _varTable, _stacks)),
        _labelInst(false)
    {
        auto&& funcBegin = module->functionBegin();
        auto&& funcEnd = module->functionEnd();
        for (auto it = funcBegin; it != funcEnd; ++it) {
            _stacks.insert({*it, Stack::New()});
        }
    }

    MipsTranslatorPtr MipsTranslator::New(llvm::ModulePtr module) {
        return std::make_shared<MipsTranslator>(module);
    }

    void MipsTranslator::translate() {
        // _allocator->allocReg();
        translate(_llvmModule);
    }

    void MipsTranslator::print(std::ostream &out) {
        // output data
        out << ".data" << std::endl;
        for (auto it = _dataTable->begin(); it != _dataTable->end(); ++it) {
            (*it).second->print(out);
            out << std::endl;
        }

        out << std::endl << ".text" << std::endl;
        for (auto inst: _mipsInsts) {
            inst->print(out);
        }

    }

    void MipsTranslator::addMipsInst(MipsInstPtr mip) {
        if (_labelInst) {
            mip->setLabel(_curLabel);
            _labelInst = false;
        }
        _mipsInsts.push_back(mip);
    }

    void MipsTranslator::translate_allocStack(int bytes) {
        bytes = -bytes; // add in opposite direction
        auto imm = MipsImm::New(bytes);
        auto rip = IInst::New(REG_SP, REG_SP, imm, MIID_ADDI);
        addMipsInst(rip);
    }

    void MipsTranslator::translate_freeStack(int bytes) {
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

        unsigned int size = _stacks[llvmFunc]->getSize();
        translate_allocStack(size);

        // basicBlock
        auto&& blockBegin = llvmFunc->blockBegin();
        auto&& blockEnd = llvmFunc->blockEnd();
        for (auto it = blockBegin; it != blockEnd; ++it) {
            translate(*it);
        }

        translate_freeStack(size);
    }

    void MipsTranslator::translate(llvm::BasicBlockPtr llvmBlock) {
        auto&& beginIter = llvmBlock->instructionBegin();
        auto&& endIter = llvmBlock->instructionEnd();
        _labelInst = true;
        _curLabel = _labelTable->findLabel(llvmBlock);
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
        auto var = _varTable->findVar(uop);

        auto usee0 = uop->getUsee(0);
        auto reg0 = readToReg(usee0, true);

        MipsRegPtr targetReg = var->getTargetReg();

        switch (uop->getUnaryOpId()) {
            case llvm::UOID_ZEXT:
                addMipsInst(RInst::NewMove(reg0, targetReg));
                break;
            case llvm::UOID_TRUNC: {
                addMipsInst(IInst::New(reg0, targetReg, IMM_TRUNC_BYTE, MIID_ANDI));
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
                addMipsInst(RInst::New(reg0, reg1, targetReg, MIID_SUB));
                break;
            case llvm::BOID_MUL:
                addMipsInst(RInst::New(reg0, reg1, REG_ZERO, MIID_MULT));
                addMipsInst(RInst::New(REG_ZERO, REG_ZERO, targetReg, MIID_MFLO));
                break;
            case llvm::BOID_DIV:
                addMipsInst(RInst::New(reg0, reg1, REG_ZERO, MIID_DIV));
                addMipsInst(RInst::New(REG_ZERO, REG_ZERO, targetReg, MIID_MFLO));
                break;
            case llvm::BOID_REM:
                addMipsInst(RInst::New(reg0, reg1, REG_ZERO, MIID_DIV));
                addMipsInst(RInst::New(REG_ZERO, REG_ZERO, targetReg, MIID_MFHI));
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
                addMipsInst(RInst::New(reg0, reg1, targetReg, MIID_SEQ));
                break;
            case llvm::CIID_NE:
                addMipsInst(RInst::New(reg0, reg1, targetReg, MIID_SNE));
                break;
            case llvm::CIID_SLE:
                addMipsInst(RInst::New(reg0, reg1, targetReg, MIID_SLE));
                break;
            case llvm::CIID_SLT:
                addMipsInst(RInst::New(reg0, reg1, targetReg, MIID_SLT));
                break;
            case llvm::CIID_SGE:
                addMipsInst(RInst::New(reg0, reg1, targetReg, MIID_SGE));
                break;
            case llvm::CIID_SGT:
                addMipsInst(RInst::New(reg0, reg1, targetReg, MIID_SGT));
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
        auto var = _varTable->findVar(lip);
        auto usee0 = lip->getUsee(0);

        auto targetReg = var->getTargetReg();
        unsigned int size = lip->getType()->getSize();
        assert(size == 1 || size == 4);

        MipsInstPtr inst0, inst1;
        if (usee0->isInst()) {
            auto inst = std::static_pointer_cast<llvm::Instruction>(usee0);
            auto prevVar = _varTable->findVar(inst);
            auto reg = readToReg(inst, true);
            unsigned int offset = prevVar->getLocation()->getOffset();
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
            auto&& _offset = prevAddrVar->getLocation()->getOffset();

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
            case llvm::PIID_CH: {
                reg = readToReg(usee0, true);
                imm = MipsImm::New(11);
                addMipsInst(RInst::NewMove(reg, REG_A0));
                addMipsInst(IInst::NewLi(REG_V0, imm));
                addMipsInst(RInst::NewSyscall());
                break;
            }
            case llvm::PIID_INT: {
                reg = readToReg(usee0, true);
                imm = MipsImm::New(1);
                addMipsInst(RInst::NewMove(reg, REG_A0));
                addMipsInst(IInst::NewLi(REG_V0, imm));
                addMipsInst(RInst::NewSyscall());
                break;
            }
            case llvm::PIID_STR: {
                MipsImmPtr strAddr = readToImm(usee0);
                imm = MipsImm::New(4);
                addMipsInst(IInst::NewLa(REG_A0, strAddr));;
                addMipsInst(IInst::NewLi(REG_V0, imm));
                addMipsInst(RInst::NewSyscall());
                break;
            }
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
        auto var = _varTable->findVar(gepip);
        auto targetReg = var->getTargetReg();

        auto usee0 = gepip->getUsee(0);
        auto usee1 = gepip->getUsee(1);
        auto ptrReg = readToReg(usee0, true);
        auto offsetReg = readToReg(usee1, false);

        auto&& ty = gepip->getType();
        assert(ty->isPointer());
        auto&& ptrTy = std::static_pointer_cast<llvm::PointerType>(ty);
        auto&& basicTy = ptrTy->getPtrBasicType();
        unsigned int size = basicTy->getSize();

        if (size == 4) {
            auto imm = MipsImm::New(2);
            addMipsInst(RInst::New(REG_ZERO, offsetReg, offsetReg, imm, MIID_SLL));
        }
        addMipsInst(RInst::New(offsetReg, ptrReg, REG_K0, MIID_ADD));
        writeBackReg(REG_K0, var);
    }

    void MipsTranslator::translate(llvm::CallInstPtr cip) {
        // move sp
        llvm::FunctionPtr func = cip->getFunc();
        unsigned int argSize = cip->getUseeSize();
        // unsigned int stackSize = 4 * (19 + argSize); // 10 tmpReg, 8 SaveReg, 1 Ra, argSize arg
        int stackSize = 4 * (1 + argSize); // 1 Ra, argSize arg
        MipsImmPtr stackOffset = MipsImm::New(stackSize);
        addMipsInst(IInst::New(REG_SP, REG_SP, stackOffset, MIID_SUBI));


        MipsImmPtr imm;
        int offset = -4;
        // store args
        for (int i = 0; i < cip->getUseeSize(); i++) {
            auto para = cip->getUsee(i);
            readToReg(para, true);

            offset += 4;
            imm = MipsImm::New(offset);
            addMipsInst(IInst::New(REG_SP, REG_K0, imm, MIID_SW));
        }
        // store ra

        offset += 4;
        imm = MipsImm::New(offset);
        addMipsInst(IInst::New(REG_SP, REG_RA, imm, MIID_SW));

        // call
        addMipsInst(JInst::New(_labelTable->findLabel(func), MIID_JAL));

        // load ra
        imm = MipsImm::New(offset);
        addMipsInst(IInst::New(REG_SP, REG_RA, imm, MIID_SW));
        offset -= 4;

        // move sp
        addMipsInst(IInst::New(REG_SP, REG_SP, stackOffset, MIID_ADDI));

        // load Value
        if (!func->getType()->isVoidTy()) {
            writeBackReg(REG_V0, _varTable->findVar(cip));
        }
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

        MipsRegPtr reg;
        if (firstOp) reg = REG_K0;
        else reg = REG_K1;

        auto stackLoc = var->getLocation();

        IInstPtr iinst;
        MipsImmPtr imm;
        imm = MipsImm::New(stackLoc->getOffset());
        if (stackLoc->getSize() == 1) {
            iinst = IInst::New(REG_SP, reg, imm, MIID_LB);
        }
        else if (stackLoc ->getSize() == 4) {
            iinst = IInst::New(REG_SP, reg, imm, MIID_LW);
        }

        addMipsInst(iinst);
        return reg;
    }

    MipsRegPtr MipsTranslator::readToReg(llvm::GlobalVariablePtr var, bool firstOp) {
        auto offset = _dataTable->findData(var);
        MipsRegPtr reg;
        if (firstOp)  reg = REG_K0;
        else reg = REG_K1;

        assert(offset->isByte() || offset->isWord());
        addMipsInst(IInst::NewLa(reg, offset));
        return reg;
    }

    MipsRegPtr MipsTranslator::readToReg(llvm::GlobalStringPtr str, bool firstOp) {
        auto offset = _dataTable->findData(str);
        MipsRegPtr reg;
        if (firstOp)  reg = REG_K0;
        else reg = REG_K1;

        assert(offset->isString());
        addMipsInst(IInst::NewLa(reg, offset));
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

    void MipsTranslator::writeBackReg(const MipsRegPtr & reg, const VariablePtr & var) {
        // TODO
    }
}
