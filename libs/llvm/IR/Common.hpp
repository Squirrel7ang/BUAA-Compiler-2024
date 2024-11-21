//
// Created by tang on 11/7/24.
//

#ifndef COMMON_HPP
#define COMMON_HPP

#include <memory>
#include <vector>

namespace llvm {

    template <typename T>
    using s_ptr = std::shared_ptr<T>;

    template <typename T>
    using u_ptr = std::unique_ptr<T>;

    template <typename T>
    using w_ptr = std::weak_ptr<T>;

    template <typename T>
    using vector = std::vector<T>;

    /******* VALUE & USES *******/

    enum ValueType {
        ARGUMENT_T,
        BASIC_BLOCK_T,

        CONSTANT_T,
        CONSTANT_DATA_T,

        FUNCTION_T,
        GLOBAL_VARIABLE_T,
        GLOBAL_STRING_T,

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
        GETINT_INST_T,
        GETCHAR_INST_T,
        PUT_INST_T,
        COMPARE_INST_T,
        INPUT_INST_T,
        OUTPUT_INST_T,
    };

    class Value;
    using ValuePtr = s_ptr<Value>;

    class LLVMContext;
    using LLVMContextPtr = s_ptr<LLVMContext>;

    class User;
    using UserPtr = s_ptr<User>;

    class Use;
    using UsePtr = s_ptr<Use>;
    using UseList = vector<UsePtr>;

    class Module;
    using ModulePtr = s_ptr<Module>;

    class Function;
    using FunctionPtr = s_ptr<Function>;

    class BasicBlock;
    using BasicBlockPtr = s_ptr<BasicBlock>;

    class Argument;
    using ArgumentPtr = s_ptr<Argument>;

    /******* TYPES *******/

    class VoidType;
    using VoidTypePtr = s_ptr<VoidType>;

    class IntegerType;
    using IntegerTypePtr = s_ptr<IntegerType>;

    class PointerType;
    using PointerTypePtr = s_ptr<PointerType>;

    class ArrayType;
    using ArrayTypePtr = s_ptr<ArrayType>;

    class LabelType;
    using LabelTypePtr = s_ptr<LabelType>;

    class FunctionType;
    using FunctionTypePtr = s_ptr<FunctionType>;

    class Type;
    using TypePtr = s_ptr<Type>;

    /******* CONSTANTS *******/

    class GlobalVariable;
    using GlobalVariablePtr = s_ptr<GlobalVariable>;

    class GlobalValue;
    using GlobalValuePtr = s_ptr<GlobalValue>;

    class Constant;
    using ConstantPtr = s_ptr<Constant>;

    class ConstantData;
    using ConstantDataPtr = s_ptr<ConstantData>;

    class GlobalString;
    using GlobalStringPtr = s_ptr<GlobalString>;

    /******* INSTRUCTIONS *******/

    class Instruction;
    using InstructionPtr = s_ptr<Instruction>;

    class UnaryInst;
    using UnaryInstPtr = s_ptr<UnaryInst>;

    class BinaryInst;
    using BinaryInstPtr = s_ptr<BinaryInst>;

    class UnaryOperator;
    using UnaryOperatorPtr = s_ptr<UnaryOperator>;

    class BinaryOperator;
    using BinaryOperatorPtr = s_ptr<BinaryOperator>;

    class CompareInst;
    using CompareInstPtr = s_ptr<CompareInst>;

    class AllocaInst;
    using AllocaInstPtr = s_ptr<AllocaInst>;

    class LoadInst;
    using LoadInstPtr = s_ptr<LoadInst>;

    class GetElePtrInst;
    using GetElePtrInstPtr = s_ptr<GetElePtrInst>;

    class StoreInst;
    using StoreInstPtr = s_ptr<StoreInst>;

    class CallInst;
    using CallInstPtr = s_ptr<CallInst>;

    class BranchInst;
    using BranchInstPtr = s_ptr<BranchInst>;

    class JumpInst;
    using JumpInstPtr = s_ptr<JumpInst>;

    class ReturnInst;
    using ReturnInstPtr = s_ptr<ReturnInst>;

    class PutInst;
    using PutInstPtr = s_ptr<PutInst>;

    class GetintInst;
    using GetIntInstPtr = s_ptr<GetintInst>;

    class GetcharInst;
    using GetCharInstPtr = s_ptr<GetcharInst>;
}

#endif //COMMON_HPP
