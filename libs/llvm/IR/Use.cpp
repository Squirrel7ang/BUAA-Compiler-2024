//
// Created by tang on 11/7/24.
//

#include "Use.hpp"

namespace llvm {
    UserPtr Use::user() { return _user; }
    ValuePtr Use::usee() { return _value; }
    ValuePtr Use::value() { return _value; }
    Use::Use(LLVMContextPtr& context, UserPtr& user, ValuePtr& value)
            :_context(context), _user(user), _value(value) {
        assert(!value->getType()->isVoidTy());
    }

}
