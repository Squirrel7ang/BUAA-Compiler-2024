//
// Created by tang on 11/6/24.
//

#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include "Common.hpp"
#include "Instruction.hpp"
#include "Type.hpp"

namespace llvm {

    class UnaryInst : public Instruction {
    public:
        explicit UnaryInst(LLVMContextPtr context, TypePtr ty,
                           ValueType vty, ValuePtr vp)
                : Instruction(context, ty, vty) {
            createUse(vp);
        }
    };

    enum UnaryOpID {
        UOID_NEG,
        UOID_NOT,
        UOID_TRUNC,
        UOID_SEXT,
    };
    class UnaryOperator : public UnaryInst {
    private:
        UnaryOpID _uoid;
    public:
        explicit UnaryOperator(LLVMContextPtr context, TypePtr ty,
                            ValuePtr vp1, UnaryOpID uoid)
                : UnaryInst(context, ty, UNARY_OPERATOR_T, vp1), _uoid(uoid) {
        }
    };

    class BinaryInst : public Instruction {
    public:
        explicit BinaryInst(LLVMContextPtr context, TypePtr ty,
                            ValueType vty, ValuePtr vp1, ValuePtr vp2)
                : Instruction(context, ty, vty) {
            createUse(vp1);
            createUse(vp2);
        }
    };

    enum BinaryOpID {
        BOID_ADD,
        BOID_SUB,
        BOID_MUL,
        BOID_DIV,
        BOID_REM,
        BOID_AND,
        BOID_OR,
    };
    class BinaryOperator : public BinaryInst {
    private:
        BinaryOpID _boid;
    public:
        explicit BinaryOperator(LLVMContextPtr& context, TypePtr ty,
                            ValuePtr vp1, ValuePtr vp2,
                            BinaryOpID boid)
                : BinaryInst(context, ty, BINARY_OPERATOR_T, vp1, vp2), _boid(boid) {
        }
    };

    class CompareInst : public BinaryInst {
    public:
        explicit CompareInst(LLVMContextPtr& context, TypePtr ty,
                            ValuePtr vp1, ValuePtr vp2)
                : BinaryInst(context, ty, COMPARE_INST_T, vp1, vp2) {
        }
    };

    class AllocaInst : public Instruction {
        TypePtr _allocaType;
    public:
        explicit AllocaInst(LLVMContextPtr& context, TypePtr ty, TypePtr allocaType)
                : Instruction(context, ty, ALLOCA_INST_T), _allocaType(allocaType) { }
        std::string output() {
            std::string ret;
            ret += "alloca ";
            ret += _type->output();
            return ret;
        }
    };

    class GetElePtrInst : public Instruction {
        TypePtr _basicTy;
        ValuePtr _ptr;
        ValuePtr _offset;
    public:
        explicit GetElePtrInst(LLVMContextPtr& context, TypePtr ty,
                               TypePtr basicTy, ValuePtr ptr, ValuePtr offset)
            : Instruction(context, ty, GETELEPTR_INST_T),
            _basicTy(basicTy), _ptr(ptr), _offset(offset){ }
    };

    class LoadInst : public UnaryInst {
    public:
        explicit LoadInst(LLVMContextPtr& context, TypePtr ty,
                          ValuePtr vp)
                : UnaryInst(context, ty, LOAD_INST_T, vp) { }
    };

    class StoreInst : public BinaryInst {
    public:
        explicit StoreInst(LLVMContextPtr& context, TypePtr ty,
                        ValuePtr value, ValuePtr ptr)
                :BinaryInst(context, ty, STORE_INST_T, value, ptr) {
        }
    };

    class CallInst : public Instruction {
    public:
        explicit CallInst(LLVMContextPtr& context, TypePtr ty, vector<ValuePtr> & args)
                : Instruction(context, ty, CALL_INST_T) {
            for (auto value: args) {
                createUse(value);
            }
        }
        explicit CallInst(LLVMContextPtr& context, TypePtr ty)
                : Instruction(context, ty, CALL_INST_T) { }
    };

    class BranchInst : public Instruction {
    public:
        explicit BranchInst(LLVMContextPtr& context, TypePtr ty,
                            ValuePtr cond, ValuePtr ifPtr, ValuePtr elsePtr)
                : Instruction(context, ty, BRANCH_INST_T){
            createUse(cond);
            createUse(ifPtr);
            createUse(elsePtr);
        }
    };

    /**
     * JumpInstruction is unconditional branch, while BranchInstruction
     * is conditional branch;
     */
    class JumpInst : public Instruction {
    public:
        explicit JumpInst(LLVMContextPtr& context, TypePtr ty,
                            ValuePtr targetPtr)
                : Instruction(context, ty, JUMP_INST_T){
            createUse(targetPtr);
        }
    };

    class ReturnInst : public Instruction {
    public:
        // return value;
        explicit ReturnInst(LLVMContextPtr& context, TypePtr ty,
                            ValuePtr ret)
                : Instruction(context, ty, RETURN_INST_T){
            createUse(ret);
        }
        // return void;
        explicit ReturnInst(LLVMContextPtr& context)
                : Instruction(context, context->VOID_TY, RETURN_INST_T){
        }
    };

    class PrintfInst : public Instruction {

    };

    class GetintInst : public Instruction {

    };

    class GetCharInst : public Instruction {

    };

}

#endif //INSTRUCTIONS_HPP
