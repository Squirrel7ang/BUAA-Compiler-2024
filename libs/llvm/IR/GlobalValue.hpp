//
// Created by tang on 11/7/24.
//

#ifndef GLOBALVALUE_HPP
#define GLOBALVALUE_HPP

#include "Common.hpp"
#include "Constant.hpp"
#include "LLVMContext.hpp"

namespace llvm {
    class GlobalValue : public Constant {
    public:
        explicit GlobalValue(LLVMContextPtr& context, TypePtr ty, ValueType vty)
                : Constant(context, ty, vty) {}
        // virtual std::string output();
    };
}

#endif //GLOBALVALUE_HPP
