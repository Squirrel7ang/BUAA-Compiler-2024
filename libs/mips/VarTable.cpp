//
// Created by tang on 11/26/24.
//

#include <cassert>

#include "VarTable.hpp"

#include "IR/Instructions.hpp"

namespace mips {
    Variable::Variable(llvm::InstructionPtr& inst, unsigned int totalCount)
        : _llvmInst(inst), _totalCounts(totalCount), _refCounts(0) {

    }

    void Variable::setLocation(VarLocationPtr loc) {
        _location = loc;
    }
}
