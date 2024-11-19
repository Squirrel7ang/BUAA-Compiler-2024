//
// Created by tang on 11/6/24.
//

#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <cassert>

#include "Common.hpp"
#include "Instruction.hpp"
#include "Type.hpp"

namespace llvm {

    class Instruction : public User {
    public:
        explicit Instruction(LLVMContextPtr& context, TypePtr ty, ValueType vty)
                :User(context, ty, vty) { }
        void printRef(std::ostream& out) override {
            out << '%' << index;
        }
    };

    class UnaryInst : public Instruction {
    public:
        explicit UnaryInst(LLVMContextPtr& context, TypePtr ty,
                           ValueType vty, ValuePtr vp)
                : Instruction(context, ty, vty) {
            createUse(vp);
        }
    };

    enum UnaryOpID {
        UOID_NEG,
        UOID_NOT,
        UOID_TRUNC,
        UOID_ZEXT, // always perform zero extend
    };
    class UnaryOperator : public UnaryInst {
    private:
        UnaryOpID _uoid;
    public:
        explicit UnaryOperator(LLVMContextPtr& context, TypePtr ty,
                            ValuePtr vp1, UnaryOpID uoid)
                : UnaryInst(context, ty, UNARY_OPERATOR_T, vp1), _uoid(uoid) {
        }
        void print(std::ostream& out) override {
            printRef(out);
            out << " = ";
            // TODO
        }
    };

    class BinaryInst : public Instruction {
    public:
        explicit BinaryInst(LLVMContextPtr& context, TypePtr ty,
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
        void print(std::ostream& out) {
            printRef(out);
            out << " = ";
            switch (_boid) {
                BOID_ADD: out << "add nsw "; break;
                BOID_SUB: out << "sub nsw "; break;
                BOID_MUL: out << "mul nsw "; break;
                BOID_DIV: out << "sdiv"; break;
                BOID_REM: out << "srem"; break;
                BOID_AND: out << "and "; break;
                BOID_OR: out << "or "; break;
                default: assert(0);
            }
            out << "i32 ";
            auto&& usee0 = getUsee(0);
            auto&& usee1 = getUsee(1);
            usee0->printRef(out);
            out << ", ";
            usee1->printRef(out);
            out << std::endl;
        }
        void printRef(std::ostream& out) override {

        }
    };

    enum CompareInstID {
        CIID_EQ,
        CIID_NE,
        CIID_SGT,
        CIID_SGE,
        CIID_SLT,
        CIID_SLE,
    };
    class CompareInst : public BinaryInst {
        CompareInstID _ciid;
    public:
        explicit CompareInst(LLVMContextPtr& context, TypePtr ty,
                            ValuePtr vp1, ValuePtr vp2, CompareInstID ciid)
                : BinaryInst(context, context->I1_TY, COMPARE_INST_T, vp1, vp2), _ciid(ciid) {
        }
        void print(std::ostream& out) override {
            printRef(out);
            out << " = ";
            out << "icmp ";
            switch (_ciid) {
                case CIID_EQ: out << "eq "; break;
                case CIID_NE: out << "ne "; break;
                case CIID_SGT: out << "sgt "; break;
                case CIID_SGE: out << "sge "; break;
                case CIID_SLT: out << "slt "; break;
                case CIID_SLE: out << "sle "; break;
                default: assert(0);
            }
            auto&& usee0 = getUsee(0);
            auto&& usee1 = getUsee(1);
            usee0->printRef(out);
            out << ", ";
            usee1->printRef(out);
            out << std::endl;
        }
    };

    class AllocaInst : public Instruction {
        TypePtr _allocaType;
    public:
        explicit AllocaInst(LLVMContextPtr& context, TypePtr ty, TypePtr allocaType)
                : Instruction(context, ty, ALLOCA_INST_T), _allocaType(allocaType) { }
        void print(std::ostream& out) {
            printRef(out);
            out << " = ";
            out << "alloca ";
            _allocaType->print(out);
            out << std::endl;
        }
    };

    class GetElePtrInst : public Instruction {
        TypePtr _basicTy;
    public:
        explicit GetElePtrInst(LLVMContextPtr& context, TypePtr ty,
                               TypePtr basicTy, ValuePtr ptr, ValuePtr offset)
            : Instruction(context, ty, GETELEPTR_INST_T),
            _basicTy(basicTy) {
            createUse(ptr);
            createUse(offset);
        }
        void print(std::ostream& out) {
            printRef(out);
            out << " = ";
            out << "getelementptr ";
            out << ", ";
            out << "ptr ";
            getUsee(0)->printRef(out);
            out << ", ";
            out << "i64 ";
            getUsee(1)->printRef(out);

            out << std::endl;
        }
    };

    class LoadInst : public UnaryInst {
        TypePtr _loadType;
    public:
        explicit LoadInst(LLVMContextPtr& context, TypePtr ty,
                          ValuePtr vp, TypePtr loadType)
                : UnaryInst(context, ty, LOAD_INST_T, vp), _loadType(loadType) { }
        void print(std::ostream& out) {
            printRef(out);
            out << " = ";
            out << "load ";
            _loadType->print(out);
            out << ", ";
            out << "ptr ";
            getUsee(0)->printRef(out);

            out << std::endl;
        }
    };

    class StoreInst : public BinaryInst {
    public:
        explicit StoreInst(LLVMContextPtr& context, TypePtr ty,
                        ValuePtr value, ValuePtr ptr)
                :BinaryInst(context, ty, STORE_INST_T, value, ptr) { }
        void print(std::ostream& out) {
            out << "store ";
            out << getUsee(0)->
            out << ", ";
            out << "ptr ";
            getUsee(0)->printRef(out);

            out << std::endl;
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
