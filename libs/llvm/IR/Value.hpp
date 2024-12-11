//
// Created by tang on 11/7/24.
//

#ifndef VALUE_HPP
#define VALUE_HPP

#include <memory>

#include "Use.hpp"
#include "Common.hpp"

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
        virtual ValueType getValueType() const { return this->_valueType; }
        virtual bool is(const ValueType vty) const { return _valueType == vty; }
        bool isInst() {
            return _valueType == UNARY_OPERATOR_T ||
                _valueType == BINARY_OPERATOR_T ||
                _valueType == COMPARE_INST_T ||
                _valueType == BRANCH_INST_T ||
                _valueType == JUMP_INST_T ||
                _valueType == LOAD_INST_T ||
                _valueType == STORE_INST_T ||
                _valueType == PUT_INST_T ||
                _valueType == GETINT_INST_T ||
                _valueType == GETCHAR_INST_T ||
                _valueType == GETELEPTR_INST_T ||
                _valueType == CALL_INST_T ||
                _valueType == ALLOCA_INST_T ||
                _valueType == RETURN_INST_T;
        }
        int userCount();
        int getIndex() { return _index; }

    protected:
        UserPtr getUser(unsigned int i);
    };
}

#endif //VALUE_HPP
