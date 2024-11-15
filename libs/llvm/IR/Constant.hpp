//
// Created by tang on 11/7/24.
//

#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include "Common.hpp"
#include "LLVMContext.hpp"
#include "User.hpp"

namespace llvm {
    class Constant: public User {
    public:
        explicit Constant(LLVMContextPtr context, TypePtr ty, ValueType vty)
                : User(context, ty, vty) {}
        // virtual std::string output();
    };
}

#endif //CONSTANT_HPP
