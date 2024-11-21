//
// Created by tang on 11/6/24.
//

#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <cassert>

#include "Common.hpp"
#include "Type.hpp"
#include "Instruction.hpp"

namespace llvm {

    class Instruction : public User {
    public:
        explicit Instruction(LLVMContextPtr& context, TypePtr ty, ValueType vty)
                :User(context, ty, vty) { }
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
                out << "i64 0 , ";
            }
            out << "i64 ";
            getUsee(1)->printRef(out);
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
            out << "ptr ";
            getUsee(0)->printRef(out);
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
    };

    class CallInst : public Instruction {
    public:
        explicit CallInst(LLVMContextPtr& context, TypePtr ty, ValuePtr func, vector<ValuePtr> & args)
                : Instruction(context, ty, CALL_INST_T) {
            createUse(func);
            for (auto value: args) {
                createUse(value);
            }
        }
        explicit CallInst(LLVMContextPtr& context, TypePtr ty, ValuePtr func)
                : Instruction(context, ty, CALL_INST_T) {
            createUse(func);
        }
        void print(std::ostream& out) {
            printRef(out);
            out << " = ";
            out << "call ";
            _type->print(out);
            out << " ";
            getUsee(0)->printRef(out);
            out << "(";

            // parameters
            for (int i = 1; i < _usees.size(); i++) {
                getUsee(i)->printRefWithType(out);
                if (i < _usees.size() - 1) {
                    out << ", ";
                }
            }

            out << ")";
        }
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
        void print(std::ostream &) override {

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
        void print(std::ostream &) override {

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
    };

}

#endif //INSTRUCTIONS_HPP
