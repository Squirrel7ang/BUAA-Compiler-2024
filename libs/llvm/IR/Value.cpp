//
// Created by tang on 11/6/24.
//

#include "Value.hpp"

namespace llvm {
    void Value::_addUser(UsePtr use) {
        _users.push_back(use);
    }

}