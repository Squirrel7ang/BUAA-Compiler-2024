//
// Created by tang on 11/6/24.
//

#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include "User.hpp"
#include "Common.hpp"

namespace llvm {

    class Instruction : public User {
    public:
        explicit Instruction(LLVMContextPtr& context, TypePtr ty, ValueType vty);
        bool is(ValueType vty) const override;
    };

    class UnaryInst : public Instruction {
    public:
        explicit UnaryInst(LLVMContextPtr& context, TypePtr ty,
                           ValueType vty, ValuePtr vp);
        bool is(ValueType vty) const override;
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
                            ValuePtr vp1, UnaryOpID uoid);
        void print(std::ostream& out) override;
        bool is(ValueType vty) const override;
        UnaryOpID getUnaryOpId() { return _uoid; }
    };

    class BinaryInst : public Instruction {
    public:
        explicit BinaryInst(LLVMContextPtr& context, TypePtr ty,
                            ValueType vty, ValuePtr vp1, ValuePtr vp2);
        bool is(ValueType vty) const override;
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
                            ValuePtr vp1, ValuePtr vp2, BinaryOpID boid);
        void print(std::ostream& out);
        bool is(ValueType vty) const override;
        BinaryOpID getBinaryOpId() { return _boid; }
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
                            ValuePtr vp1, ValuePtr vp2, CompareInstID ciid);
        void print(std::ostream& out) override;
        bool is(ValueType vty) const override;
        CompareInstID getCompareId() { return _ciid; }
    };

    class AllocaInst : public Instruction {
        TypePtr _allocaType;
    public:
        explicit AllocaInst(LLVMContextPtr& context, TypePtr ty, TypePtr allocaType);
        void print(std::ostream& out);
        bool is(ValueType vty) const override;
        int allocateSpace() const;
    };

    class GetElePtrInst : public Instruction {
    public:
        explicit GetElePtrInst(LLVMContextPtr& context, TypePtr ty,
                               ValuePtr ptr, ValuePtr offset);
        void print(std::ostream& out);
        bool is(ValueType vty) const override;
    };

    class LoadInst : public UnaryInst {
    public:
        explicit LoadInst(LLVMContextPtr& context, TypePtr ty, ValuePtr vp);
        void print(std::ostream& out);
        bool is(ValueType vty) const override;
    };

    class StoreInst : public BinaryInst {
    public:
        explicit StoreInst(LLVMContextPtr& context,
                        ValuePtr value, ValuePtr ptr);
        void print(std::ostream& out);
        bool is(ValueType vty) const override;
    };

    class CallInst : public Instruction {
        FunctionPtr _func;
    public:
        explicit CallInst(LLVMContextPtr& context, TypePtr ty, ValuePtr func, vector<ValuePtr> & args);
        explicit CallInst(LLVMContextPtr& context, TypePtr ty, ValuePtr func);
        FunctionPtr getFunc() { return _func; }
        void print(std::ostream& out) override;
        bool is(ValueType vty) const override;
    };

    class BranchInst : public Instruction {
    public:
        explicit BranchInst(LLVMContextPtr& context,
                            ValuePtr cond, ValuePtr trueBlock, ValuePtr falseBlock);
        void print(std::ostream& out) override;
        BasicBlockPtr getTarget(bool cond);
        bool is(ValueType vty) const override;
    };

    /**
     * JumpInstruction is unconditional branch, while BranchInstruction
     * is conditional branch;
     */
    class JumpInst : public Instruction {
    public:
        explicit JumpInst(LLVMContextPtr& context, ValuePtr targetPtr);
        void print(std::ostream& out) override;
        BasicBlockPtr getTarget();
        bool is(ValueType vty) const override;
    };

    class ReturnInst : public Instruction {
    public:
        // return value;
        explicit ReturnInst(LLVMContextPtr& context, ValuePtr ret);
        // return void;
        explicit ReturnInst(LLVMContextPtr& context);
        void print(std::ostream & out) override;
        bool is(ValueType vty) const override;
    };

    enum PutInstID {
        PIID_CH,
        PIID_INT,
        PIID_STR,
    };
    class PutInst : public Instruction {
        PutInstID _piid;
    public:
        explicit PutInst(LLVMContextPtr& context, ValuePtr value, PutInstID piid);
        void print(std::ostream &out) override;
        bool is(ValueType vty) const override;
        PutInstID getPutID() { return _piid; }
    };

    class GetintInst : public Instruction {
    public:
        explicit GetintInst(LLVMContextPtr context);
        void print(std::ostream& out) override;
        bool is(ValueType vty) const override;
    };

    class GetcharInst : public Instruction {
    public:
        explicit  GetcharInst(LLVMContextPtr context);
        void print(std::ostream& out) override;
        bool is(ValueType vty) const override;
    };

}

#endif //INSTRUCTIONS_HPP
