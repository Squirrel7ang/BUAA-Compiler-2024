//
// Created by tang on 11/6/24.
//

#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <cassert>

#include "User.hpp"
#include "Function.hpp"
#include "Common.hpp"
#include "Type.hpp"

namespace llvm {

    class Instruction : public User {
    public:
        explicit Instruction(LLVMContextPtr& context, TypePtr ty, ValueType vty)
                :User(context, ty, vty) { }
        bool is(const ValueType vty) const override {
            return vty == UNARY_OPERATOR_T ||
                vty == BINARY_OPERATOR_T ||
                vty == COMPARE_INST_T ||
                vty == BRANCH_INST_T ||
                vty == JUMP_INST_T ||
                vty == LOAD_INST_T ||
                vty == STORE_INST_T ||
                vty == PUT_INST_T ||
                vty == GETINT_INST_T ||
                vty == GETCHAR_INST_T ||
                vty == GETELEPTR_INST_T ||
                vty == CALL_INST_T ||
                vty == ALLOCA_INST_T ||
                vty == RETURN_INST_T;
        }
    };

    class UnaryInst : public Instruction {
    public:
        explicit UnaryInst(LLVMContextPtr& context, TypePtr ty,
                           ValueType vty, ValuePtr vp)
                : Instruction(context, ty, vty) {
            createUse(vp);
        }
        bool is(const ValueType vty) const override {
            return vty == UNARY_OPERATOR_T ||
                vty == LOAD_INST_T;
        }
    };

    enum UnaryOpID {
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
            switch (_uoid) {
                case(UOID_ZEXT): out << "zext "; break;
                case(UOID_TRUNC): out << "trunc "; break;
                default: assert(0);
            }
            getUsee(0)->printRefWithType(out);
            out << " to ";
            _type->print(out);
        }
        bool is(const ValueType vty) const override {
            return vty == UNARY_OPERATOR_T;
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
        bool is(const ValueType vty) const override {
            return vty == BINARY_OPERATOR_T ||
                vty == STORE_INST_T ||
                vty == COMPARE_INST_T;
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
                case BOID_ADD: out << "add nsw "; break;
                case BOID_SUB: out << "sub nsw "; break;
                case BOID_MUL: out << "mul nsw "; break;
                case BOID_DIV: out << "sdiv "; break;
                case BOID_REM: out << "srem "; break;
                default: assert(0);
            }
            _type->print(out);
            out << " ";
            getUsee(0)->printRef(out);
            out << ", ";
            getUsee(1)->printRef(out);
        }
        bool is(const ValueType vty) const override {
            return vty == BINARY_OPERATOR_T;
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
        explicit CompareInst(LLVMContextPtr& context,
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
            usee0->printRefWithType(out);
            out << ", ";
            usee1->printRef(out);
        }
        bool is(const ValueType vty) const override {
            return vty == COMPARE_INST_T;
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
        }
        bool is(const ValueType vty) const override {
            return vty == ALLOCA_INST_T;
        }
        int allocateSpace() const;
    };

    class GetElePtrInst : public Instruction {
    public:
        explicit GetElePtrInst(LLVMContextPtr& context, TypePtr ty,
                               ValuePtr ptr, ValuePtr offset)
            : Instruction(context, ty, GETELEPTR_INST_T) {
            createUse(ptr);
            createUse(offset);
        }
        void print(std::ostream& out) {
            printRef(out);
            out << " = ";
            out << "getelementptr ";

            out << "inbounds ";
            auto&& usee0 = getUsee(0);
            assert(usee0->getType()->isPointer());
            auto&& pty = std::static_pointer_cast<PointerType>(usee0->getType());
            pty->getBasicType()->print(out);
            out << ", ";

            usee0->printRefWithType(out);
            out << ", ";
            if (pty->getBasicType()->isArray()) {
                out << "i32 0 , ";
            }
            out << "i32 ";
            getUsee(1)->printRef(out);
        }
        bool is(const ValueType vty) const override {
            return vty == GETELEPTR_INST_T;
        }
    };

    class LoadInst : public UnaryInst {
    public:
        explicit LoadInst(LLVMContextPtr& context, TypePtr ty, ValuePtr vp)
                : UnaryInst(context, ty, LOAD_INST_T, vp) {
            assert(vp->getType()->isPointer());
        }
        void print(std::ostream& out) {
            printRef(out);
            out << " = ";
            out << "load ";
            _type->print(out);
            out << ", ";
            getUsee(0)->printRefWithType(out);
        }
        bool is(const ValueType vty) const override {
            return vty == LOAD_INST_T;
        }
    };

    class StoreInst : public BinaryInst {
    public:
        explicit StoreInst(LLVMContextPtr& context,
                        ValuePtr value, ValuePtr ptr)
                :BinaryInst(context, context->VOID_TY, STORE_INST_T, value, ptr) { }
        void print(std::ostream& out) {
            out << "store ";
            getUsee(0)->printRefWithType(out);
            out << ", ";
            getUsee(1)->printRefWithType(out);
        }
        bool is(const ValueType vty) const override {
            return vty == STORE_INST_T;
        }
    };

    class CallInst : public Instruction {
        FunctionPtr _func;
    public:
        explicit CallInst(LLVMContextPtr& context, TypePtr ty, ValuePtr func, vector<ValuePtr> & args)
                : Instruction(context, ty, CALL_INST_T), _func(std::dynamic_pointer_cast<Function>(func)) {
            // createUse(func);
            for (auto value: args) {
                createUse(value);
            }
        }
        explicit CallInst(LLVMContextPtr& context, TypePtr ty, ValuePtr func)
                : Instruction(context, ty, CALL_INST_T), _func(std::dynamic_pointer_cast<Function>(func)) {
            // createUse(func);
        }
        void print(std::ostream& out) override;
        bool is(const ValueType vty) const override {
            return vty == CALL_INST_T;
        }
    };

    class BranchInst : public Instruction {
    public:
        explicit BranchInst(LLVMContextPtr& context,
                            ValuePtr cond, ValuePtr trueBlock, ValuePtr falseBlock)
                : Instruction(context, context->VOID_TY, BRANCH_INST_T){
            createUse(cond);
            createUse(trueBlock);
            createUse(falseBlock);
        }
        void print(std::ostream& out) override {
            out << "br ";
            getUsee(0)->printRefWithType(out);
            out << ", ";
            getUsee(1)->printRefWithType(out);
            out << ", ";
            getUsee(2)->printRefWithType(out);
        }
        BasicBlockPtr getTarget(bool cond) {
            int i = cond ? 1 : 0;
            auto t = getUsee(i + 1);
            assert(t->is(BASIC_BLOCK_T));
            auto targetBlock = std::static_pointer_cast<BasicBlock>(t);
            return targetBlock;
        }
        bool is(const ValueType vty) const override {
            return vty == BRANCH_INST_T;
        }
    };

    /**
     * JumpInstruction is unconditional branch, while BranchInstruction
     * is conditional branch;
     */
    class JumpInst : public Instruction {
    public:
        explicit JumpInst(LLVMContextPtr& context, ValuePtr targetPtr)
                : Instruction(context, context->VOID_TY, JUMP_INST_T){
            createUse(targetPtr);
        }
        void print(std::ostream& out) override {
            out << "br ";
            getUsee(0)->printRefWithType(out);
        }
        BasicBlockPtr getTarget() {
            auto t = getUsee(0);
            assert(t->is(BASIC_BLOCK_T));
            auto targetBlock = std::static_pointer_cast<BasicBlock>(t);
            return targetBlock;
        }
        bool is(const ValueType vty) const override {
            return vty == JUMP_INST_T;
        }
    };

    class ReturnInst : public Instruction {
    public:
        // return value;
        explicit ReturnInst(LLVMContextPtr& context, ValuePtr ret)
                : Instruction(context, context->VOID_TY, RETURN_INST_T){
            createUse(ret);
        }
        // return void;
        explicit ReturnInst(LLVMContextPtr& context)
                : Instruction(context, context->VOID_TY, RETURN_INST_T){
        }
        void print(std::ostream & out) override {
            out << "ret ";
            if (_usees.size() > 0) {
                getUsee(0)->printRefWithType(out);
            }
            else {
                _context->VOID_TY->print(out);
            }
        }
        bool is(const ValueType vty) const override {
            return vty == RETURN_INST_T;
        }
    };

    enum PutInstID {
        PIID_CH,
        PIID_INT,
        PIID_STR,
    };
    class PutInst : public Instruction {
        PutInstID _piid;
    public:
        explicit PutInst(LLVMContextPtr& context, ValuePtr value, PutInstID piid)
                : Instruction(context, context->VOID_TY, PUT_INST_T), _piid(piid) {
            createUse(value);
        }
        void print(std::ostream &out) override {
            out << "call void @";
            switch (_piid) {
                case PIID_CH: out << "putch"; break;
                case PIID_INT: out << "putint"; break;
                case PIID_STR: out << "putstr"; break;
                default: assert(0);
            }
            out << "(";
            getUsee(0)->printRefWithType(out);
            out << ")";
        }
        bool is(const ValueType vty) const override {
            return vty == PUT_INST_T;
        }
    };

    class GetintInst : public Instruction {
    public:
        explicit  GetintInst(LLVMContextPtr context)
                : Instruction(context, context->I32_TY, GETINT_INST_T) { }
        void print(std::ostream& out) override {
            printRef(out);
            out << " = ";
            out << "call ";
            _type->print(out);
            out << "@getint()";
        }
        bool is(const ValueType vty) const override {
            return vty == GETINT_INST_T;
        }
    };

    class GetcharInst : public Instruction {
    public:
        explicit  GetcharInst(LLVMContextPtr context)
                : Instruction(context, context->I32_TY, GETCHAR_INST_T) { }
        void print(std::ostream& out) override {
            printRef(out);
            out << " = ";
            out << "call ";
            _type->print(out);
            out << "@getchar()";
        }
        bool is(const ValueType vty) const override {
            return vty == GETCHAR_INST_T;
        }
    };

}

#endif //INSTRUCTIONS_HPP
