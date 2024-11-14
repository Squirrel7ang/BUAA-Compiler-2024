//
// Created by tang on 11/6/24.
//

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "Common.hpp"
#include "User.hpp"

namespace llvm {
    class Instruction : public User {
    public:
        explicit Instruction(LLVMContextPtr context, TypePtr ty, ValueType vty)
                :User(context, ty, vty) { }

    };
}

#endif //INSTRUCTION_HPP
