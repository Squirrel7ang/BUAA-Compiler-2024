//
// Created by tang on 11/7/24.
//

#include <cassert>

#include "User.hpp"
#include "Use.hpp"

namespace llvm {
    void User::createUse(ValuePtr val) {
        UserPtr me = std::shared_ptr<User>(this);
        UsePtr use = std::make_shared<Use>(_context, me, val);

        // push into user's usees
        _usees.push_back(use);

        // push into usee's users
        val->_addUser(use);

        // push into Global Uselist;
        _context->addUse(use);
    }

    ValuePtr User::OpAt(unsigned int i) {
        return _usees.at(i)->usee();
    }

}