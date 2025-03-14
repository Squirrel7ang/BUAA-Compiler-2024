//
// Created by tang on 11/6/24.
//

#ifndef USER_HPP
#define USER_HPP

#include <memory>

#include "Value.hpp"
#include "LLVMContext.hpp"
#include "Common.hpp"

namespace llvm {
    class User : public Value {
    protected:
        vector<UsePtr> _usees;
    public:
        explicit User(LLVMContextPtr& context, TypePtr& ty, ValueType vty)
                : Value(context, ty, vty) {}
        unsigned int getUseeSize();
        ValuePtr getUsee(unsigned int i);
    protected:
        void createUse(ValuePtr val);
        ValuePtr OpAt(unsigned int i);
    };
}

#endif //USER_HPP
