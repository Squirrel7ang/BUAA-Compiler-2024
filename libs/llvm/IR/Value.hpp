//
// Created by tang on 11/7/24.
//

#ifndef VALUE_HPP
#define VALUE_HPP

#include <memory>

#include "Common.hpp"
#include "Use.hpp"

namespace llvm {
    class LLVMContext;

    class Value {
    protected:
        LLVMContextPtr _context;
        TypePtr _type;
        ValueType _valueType;
        vector<UsePtr> _users;
        int _index = -1; // the tmp variable's index
    public:
        virtual ~Value() = default;
        explicit Value(LLVMContextPtr& context, TypePtr ty, ValueType valueType);
        virtual void print(std::ostream& out);
        virtual void printRef(std::ostream& out);
        virtual void setIndex(int& index);
        void _addUser(UsePtr use);
        void printType(std::ostream& out);
        void printRefWithType(std::ostream& out);
        virtual TypePtr getType() { return this->_type; }
        bool is(const ValueType vty) const { return _valueType == vty; }

    protected:
        UserPtr getUser(unsigned int i);
    };
}

#endif //VALUE_HPP
