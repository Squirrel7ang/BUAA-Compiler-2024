//
// Created by tang on 11/6/24.
//

#ifndef USER_HPP
#define USER_HPP

#include <memory>

#include "Common.hpp"
#include "Value.hpp"
#include "LLVMContext.hpp"

namespace llvm {
    class User : public Value{
    protected:
        vector<UsePtr> _usees;
    public:
        explicit User(LLVMContextPtr context, TypePtr ty, ValueType vty): Value(context, ty, vty) {}
        // virtual std::string output();
    protected:
        void createUse(ValuePtr val);
    };
}

#endif //USER_HPP
