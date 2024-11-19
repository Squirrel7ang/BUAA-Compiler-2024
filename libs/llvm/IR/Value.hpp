//
// Created by tang on 11/7/24.
//

#ifndef VALUE_HPP
#define VALUE_HPP

#include <memory>

#include "Common.hpp"
#include "LLVMContext.hpp"
#include "Use.hpp"

namespace llvm {
    class LLVMContext;
    enum ValueType {
        ARGUMENT_T,
        BASIC_BLOCK_T,

        CONSTANT_T,
        CONSTANT_DATA_T,

        FUNCTION_T,
        GLOBAL_VARIABLE_T,

        BINARY_INST_T,
        BINARY_OPERATOR_T,
        JUMP_INST_T,
        RETURN_INST_T,
        UNARY_INST_T,
        UNARY_OPERATOR_T,
        BRANCH_INST_T,
        CALL_INST_T,
        LOAD_INST_T,
        STORE_INST_T,
        ALLOCA_INST_T,
        GETELEPTR_INST_T,
        COMPARE_INST_T,
        INPUT_INST_T,
        OUTPUT_INST_T,
    };

    class Value {
    protected:
        LLVMContextPtr _context;
        TypePtr _type;
        ValueType _valueType;
        vector<UsePtr> _users;
        int index = -1; // the tmp variable's index
    public:
        explicit Value(LLVMContextPtr& context, TypePtr ty, ValueType valueType)
                : _context(context), _valueType(valueType) {}
        virtual void print(std::ostream&) { }
        virtual void printRef(std::ostream&) { }
        void _addUser(UsePtr use);

    protected:
        UserPtr getUser(unsigned int i) {
            return _users.at(i)->user();
        }

    protected:
    };
}

#endif //VALUE_HPP
