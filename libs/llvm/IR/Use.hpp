//
// Created by tang on 11/7/24.
//

#ifndef USE_HPP
#define USE_HPP

#include <memory>

#include "Common.hpp"

namespace llvm {
    class Use {
    protected:
        LLVMContextPtr _context;
        UserPtr _user;
        ValuePtr _value;
    public:
        explicit Use(LLVMContextPtr& context, UserPtr& user, ValuePtr& value);
        UserPtr user();
        ValuePtr usee();
        ValuePtr value();
    };
}

#endif //USE_HPP
