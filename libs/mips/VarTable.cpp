//
// Created by tang on 11/26/24.
//

#include <cassert>

#include "VarTable.hpp"

#include "IR/Instructions.hpp"
#include "IR/Module.hpp"

namespace mips {
    void Variable::addConflictVar(VariablePtr var) {
        next.push_back(var);
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
                    VariablePtr&& vp = Variable::New(*i);
                    addVariable(*i, vp);
                }
            }
        }
    }

    void VarTable::addVariable(llvm::InstructionPtr& inst, VariablePtr& var) {
        _vars.insert({inst, var});
    }

    VariablePtr VarTable::findVar(llvm::InstructionPtr inst) {
        return _vars[inst];
    }

    VarTablePtr VarTable::New(llvm::ModulePtr module) {
        return std::make_shared<VarTable>(module);
    }
}
