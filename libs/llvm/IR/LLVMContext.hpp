//
// Created by tang on 11/7/24.
//

#ifndef LLVMCONTEXT_HPP
#define LLVMCONTEXT_HPP

#include <vector>

#include "Common.hpp"
#include "Type.hpp"
#include "Use.hpp"

namespace llvm {
    class Value;

    class LLVMContext {
        std::vector<ValuePtr> value;
        std::vector<UsePtr> uses;
    public:
        IntegerTypePtr I32_TY;
        IntegerTypePtr I8_TY;
        explicit LLVMContext() {
            LLVMContextPtr me = LLVMContextPtr(this);
            I32_TY = std::make_shared<IntegerType>(me, 4);
            I8_TY = std::make_shared<IntegerType>(me, 1);
        }
        void addUse(UsePtr use);
    };
}

#endif //LLVMCONTEXT_HPP
