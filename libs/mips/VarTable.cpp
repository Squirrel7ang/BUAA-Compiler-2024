//
// Created by tang on 11/26/24.
//

#include <cassert>

#include "VarTable.hpp"

#include "Regs.hpp"
#include "VarLocation.hpp"
#include "IR/Instructions.hpp"
#include "IR/Module.hpp"

namespace mips {
    void Variable::addConflictVar(VariablePtr var) {
        next.insert(var);
    }

    VariablePtr Variable::removeConflictVar(VariablePtr vp) {
        for (auto it = next.begin(); it != next.end(); ++it) {
            if (*it == vp) {
                next.erase(it);
                break;
            }
        }
        return vp;
    }

    MipsRegPtr Variable::getTargetReg() {
        MipsRegPtr targetReg;
        if (_location->is(VLID_REG)) {
            targetReg = std::static_pointer_cast<MipsReg>(_location);
        }
        else {
            targetReg = REG_K0;
        }
        return targetReg;
    }

    bool Variable::hasLocation() {
        return _location != nullptr;
    }

    int Variable::size() {
        return _llvmInst->getType()->getSize();
    }

    void Variable::countOneRef() {
        _refCounts++;
    }

    bool Variable::reachRefCount() {
        if (_refCounts > _totalCounts) {
            assert(0);
        }
        return _refCounts == _totalCounts;
    }

    VariablePtr Variable::New(llvm::InstructionPtr inst) {
        return std::make_shared<Variable>(inst, inst->userCount());
    }

    Variable::Variable(llvm::InstructionPtr& inst, unsigned int totalCount)
        : _llvmInst(inst), _totalCounts(totalCount), _refCounts(0) {
    }

    void Variable::setLocation(VarLocationPtr loc) {
        _location = loc;
    }

    void Variable::setOffset(unsigned int offset) {
        assert(_location->is(VLID_REG));
        auto reg = std::static_pointer_cast<MipsReg>(_location);
        auto newReg = MipsReg::New(reg->getRegNum(), offset);
        setLocation(newReg);
    }

    VarTable::VarTable(llvm::ModulePtr& module) {
        auto&& funcBegin = module->functionBegin();
        auto&& funcEnd = module->functionEnd();
        for (auto f = funcBegin; f != funcEnd; ++f) {
            auto&& blockBegin = (*f)->blockBegin();
            auto&& blockEnd = (*f)->blockEnd();
            for (auto b = blockBegin; b != blockEnd; ++b) {
                auto&& instBegin = (*b)->instructionBegin();
                auto&& instEnd = (*b)->instructionEnd();
                for (auto i = instBegin; i != instEnd; ++i) {
                    if (!(*i)->getType()->isVoidTy()) {
                        VariablePtr&& vp = Variable::New(*i);
                        addVariable(*i, vp);
                    }
                }
            }
        }
    }

    void VarTable::addVariable(llvm::InstructionPtr& inst, VariablePtr& var) {
        _vars.insert({inst, var});
    }

    VariablePtr VarTable::findVar(llvm::InstructionPtr inst) {
        if (_vars.contains(inst)) {
            return _vars[inst];
        }
        else {
            assert(0);
            return nullptr;
        }
    }

    std::map<llvm::InstructionPtr, VariablePtr>::iterator VarTable::begin() {
        return _vars.begin();
    }

    std::map<llvm::InstructionPtr, VariablePtr>::iterator VarTable::end() {
        return _vars.begin();
    }

    VarTablePtr VarTable::New(llvm::ModulePtr module) {
        return std::make_shared<VarTable>(module);
    }
}
