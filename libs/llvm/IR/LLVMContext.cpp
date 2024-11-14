//
// Created by tang on 11/7/24.
//

#include "LLVMContext.hpp"

namespace llvm {
    void LLVMContext::addUse(UsePtr use) {
        uses.push_back(use);
    }
}
