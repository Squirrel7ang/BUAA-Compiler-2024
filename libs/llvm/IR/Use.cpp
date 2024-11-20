//
// Created by tang on 11/7/24.
//

#include "Use.hpp"

namespace llvm {
    UserPtr Use::user() { return _user; }
    ValuePtr Use::usee() { return _value; }
    ValuePtr Use::value() { return _value; }

}
