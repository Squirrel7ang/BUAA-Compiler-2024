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
        IntegerTypePtr I1_TY;
        VoidTypePtr VOID_TY;
        PointerTypePtr I32_PTR_TY;
        PointerTypePtr I8_PTR_TY;
        LabelTypePtr LABEL_TY;
        explicit LLVMContext() {
            this->I32_TY = std::make_shared<IntegerType>(32);
            this->I8_TY = std::make_shared<IntegerType>(8);
            this->I1_TY = std::make_shared<IntegerType>(1);
            VOID_TY = std::make_shared<VoidType>();
            I32_PTR_TY = std::make_shared<PointerType>(I32_TY);
            I8_PTR_TY = std::make_shared<PointerType>(I8_TY);
            sourceName = "testfile.txt";
        }
        void addUse(UsePtr use);

        // Printer
        std::string getSourceName() { return sourceName; }
        std::string getStringName() {
            static int i = -1;
            i++;
            return "str." + std::to_string(i);
        }
    };
}

#endif //LLVMCONTEXT_HPP
