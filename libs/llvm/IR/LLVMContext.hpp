//
// Created by tang on 11/7/24.
//

#ifndef LLVMCONTEXT_HPP
#define LLVMCONTEXT_HPP

#include <vector>

#include "Common.hpp"
#include "Type.hpp"

namespace llvm {
    class Value;

    class LLVMContext {
    protected:
        std::vector<ValuePtr> value;
        std::vector<UsePtr> uses;
        std::string sourceName;
    public:
        IntegerTypePtr I32_TY;
        IntegerTypePtr I8_TY;
        VoidTypePtr VOID_TY;
        PointerTypePtr I32_PTR_TY;
        PointerTypePtr I8_PTR_TY;
        LabelTypePtr LABEL_TY;
        explicit LLVMContext() {
            LLVMContextPtr me = LLVMContextPtr(this);
            I32_TY = std::make_shared<IntegerType>(me, 4);
            I8_TY = std::make_shared<IntegerType>(me, 1);
            VOID_TY = std::make_shared<VoidType>(me);
            I32_PTR_TY = std::make_shared<PointerType>(me, I32_TY);
            I8_PTR_TY = std::make_shared<PointerType>(me, I8_TY);
            sourceName = "testfile.txt";
        }
        void addUse(UsePtr use);
    };
}

#endif //LLVMCONTEXT_HPP
