//
// Created by tang on 11/26/24.
//

#include <cassert>

#include "VarTable.hpp"

#include "Regs.hpp"
#include "IR/Instructions.hpp"
#include "IR/Module.hpp"
#include "IR/GlobalVariable.hpp"
#include "IR/GlobalString.hpp"
#include "IR/Value.hpp"

namespace mips {
    bool Variable::hasLocation() {
        return _location != nullptr;
    }

    void Variable::setLocation(StackSlotPtr slot) {
        _location = slot;
    }

    MipsRegPtr Variable::getTargetReg() {
        return REG_K0;
    }

    VariablePtr Variable::New(llvm::ValuePtr val) {
        return std::make_shared<Variable>(val);
    }

    Variable::Variable(llvm::ValuePtr& inst)
        : _llvmVal(inst) {
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
                        addVariable(*i, Variable::New(*i));
                    }
                }
            }
            auto&& argBegin = (*f)->argumentBegin();
            auto&& argEnd = (*f)->argumentEnd();
            for (auto a = argBegin; a != argEnd; ++a) {
                addVariable(*a, Variable::New(*a));
            }
        }

    }

    int Variable::size() {
        return _llvmVal->getType()->getSize();
    }

    void VarTable::addVariable(llvm::ValuePtr val, VariablePtr var) {
        _vars.insert({val, var});
    }

    VariablePtr VarTable::findVar(llvm::ValuePtr val) {
        if (_vars.contains(val)) {
            return _vars[val];
        }
        else {
            assert(0);
            return nullptr;
        }
    }

    VarTablePtr VarTable::New(llvm::ModulePtr module) {
        return std::make_shared<VarTable>(module);
    }
}
