//
// Created by tang on 11/7/24.
//

#ifndef USE_HPP
#define USE_HPP

#include <vector>
#include <memory>

#include "Common.hpp"
#include "Value.hpp"
#include "User.hpp"

namespace llvm {
    class Use {
    protected:
        LLVMContextPtr _context;
        UserPtr _user;
        ValuePtr _value;
    public:
        explicit Use(LLVMContextPtr& context, UserPtr user, ValuePtr value)
                :_context(context), _user(user), _value(value) { }
        UserPtr user() { return _user; }
        ValuePtr usee() { return _value; }
        ValuePtr value() { return _value; }
    };
}

#endif //USE_HPP
