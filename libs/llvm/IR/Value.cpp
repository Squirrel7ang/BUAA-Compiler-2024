//
// Created by tang on 11/6/24.
//

#include "Common.hpp"
#include "Type.hpp"
#include "Value.hpp"

namespace llvm {
    void Value::_addUser(UsePtr use) {
        _users.push_back(use);
    }

    UserPtr Value::getUser(unsigned int i) {
        return _users.at(i)->user();
    }

    void Value::printRefWithType(std::ostream& out) {
        printType(out);
        out << ' ';
        printRef(out);
    }

    void Value::printType(std::ostream& out) {
        _type->print(out);
    }

    Value::Value(LLVMContextPtr& context, TypePtr ty, ValueType valueType)
            : _context(context), _valueType(valueType) {
        _type = ty;
    }

    void Value::print(std::ostream& out) { }

    void Value::printRef(std::ostream& out) {
        out << "%" << _index;
    }

    void Value::setIndex(int& index) {
        _index = index;
        index++;
    }
}
