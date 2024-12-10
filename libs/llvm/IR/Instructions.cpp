//
// Created by tang on 11/6/24.
//

#include "Instructions.hpp"

#include "Function.hpp"

namespace llvm {

    Instruction::Instruction(LLVMContextPtr& context, TypePtr ty, ValueType vty)
            :User(context, ty, vty) { }

    bool Instruction::is(const ValueType vty) const {
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

    UnaryInst::UnaryInst(LLVMContextPtr& context, TypePtr ty,
                       ValueType vty, ValuePtr vp)
            : Instruction(context, ty, vty) {
        createUse(vp);
    }

    bool UnaryInst::is(const ValueType vty) const {
        return vty == UNARY_OPERATOR_T ||
            vty == LOAD_INST_T;
    }

    UnaryOperator::UnaryOperator(LLVMContextPtr& context, TypePtr ty,
                        ValuePtr vp1, UnaryOpID uoid)
            : UnaryInst(context, ty, UNARY_OPERATOR_T, vp1), _uoid(uoid) {
    }

    void UnaryOperator::print(std::ostream& out) {
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

    bool UnaryOperator::is(const ValueType vty) const {
        return vty == UNARY_OPERATOR_T;
    }

    BinaryInst::BinaryInst(LLVMContextPtr& context, TypePtr ty,
                        ValueType vty, ValuePtr vp1, ValuePtr vp2)
            : Instruction(context, ty, vty) {
        createUse(vp1);
        createUse(vp2);
    }

    bool BinaryInst::is(const ValueType vty) const {
        return vty == BINARY_OPERATOR_T ||
            vty == STORE_INST_T ||
            vty == COMPARE_INST_T;
    }

    BinaryOperator::BinaryOperator(LLVMContextPtr& context, TypePtr ty,
                        ValuePtr vp1, ValuePtr vp2,
                        BinaryOpID boid)
            : BinaryInst(context, ty, BINARY_OPERATOR_T, vp1, vp2), _boid(boid) {
    }

    void BinaryOperator::print(std::ostream& out) {
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

    bool BinaryOperator::is(const ValueType vty) const {
        return vty == BINARY_OPERATOR_T;
    }

    CompareInst::CompareInst(LLVMContextPtr& context,
                        ValuePtr vp1, ValuePtr vp2, CompareInstID ciid)
            : BinaryInst(context, context->I1_TY, COMPARE_INST_T, vp1, vp2), _ciid(ciid) {
    }

    void CompareInst::print(std::ostream& out) {
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

    bool CompareInst::is(const ValueType vty) const {
        return vty == COMPARE_INST_T;
    }

    AllocaInst::AllocaInst(LLVMContextPtr& context, TypePtr ty, TypePtr allocaType)
            : Instruction(context, ty, ALLOCA_INST_T), _allocaType(allocaType) { }

    void AllocaInst::print(std::ostream& out) {
        printRef(out);
        out << " = ";
        out << "alloca ";
        _allocaType->print(out);
    }

    bool AllocaInst::is(const ValueType vty) const {
        return vty == ALLOCA_INST_T;
    }

    int AllocaInst::allocateSpace() const {
        return _allocaType->getSize();
    }

    GetElePtrInst::GetElePtrInst(LLVMContextPtr& context, TypePtr ty,
                           ValuePtr ptr, ValuePtr offset)
        : Instruction(context, ty, GETELEPTR_INST_T) {
        createUse(ptr);
        createUse(offset);
    }

    void GetElePtrInst::print(std::ostream& out) {
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

    bool GetElePtrInst::is(const ValueType vty) const {
        return vty == GETELEPTR_INST_T;
    }

    LoadInst::LoadInst(LLVMContextPtr& context, TypePtr ty, ValuePtr vp)
            : UnaryInst(context, ty, LOAD_INST_T, vp) {
        assert(vp->getType()->isPointer());
    }

    void LoadInst::print(std::ostream& out) {
        printRef(out);
        out << " = ";
        out << "load ";
        _type->print(out);
        out << ", ";
        getUsee(0)->printRefWithType(out);
    }

    bool LoadInst::is(const ValueType vty) const {
        return vty == LOAD_INST_T;
    }

    StoreInst::StoreInst(LLVMContextPtr& context,
                    ValuePtr value, ValuePtr ptr)
            :BinaryInst(context, context->VOID_TY, STORE_INST_T, value, ptr) { }

    void StoreInst::print(std::ostream& out) {
        out << "store ";
        getUsee(0)->printRefWithType(out);
        out << ", ";
        getUsee(1)->printRefWithType(out);
    }

    bool StoreInst::is(const ValueType vty) const {
        return vty == STORE_INST_T;
    }

    CallInst::CallInst(LLVMContextPtr& context, TypePtr ty, ValuePtr func, vector<ValuePtr> & args)
            : Instruction(context, ty, CALL_INST_T), _func(std::dynamic_pointer_cast<Function>(func)) {
        // createUse(func);
        for (auto value: args) {
            createUse(value);
        }
    }

    CallInst::CallInst(LLVMContextPtr& context, TypePtr ty, ValuePtr func)
            : Instruction(context, ty, CALL_INST_T), _func(std::dynamic_pointer_cast<Function>(func)) {
        // createUse(func);
    }

    void CallInst::print(std::ostream& out) {
        if (!_type->isVoidTy()) {
            printRef(out);
            out << " = ";
        }
        out << "call ";
        _type->print(out);
        out << " ";
        _func->printRef(out);
        out << "(";

        // parameters
        for (int i = 0; i < _usees.size(); i++) {
            getUsee(i)->printRefWithType(out);
            if (i < _usees.size() - 1) {
                out << ", ";
            }
        }

        out << ")";
    }

    bool CallInst::is(const ValueType vty) const {
        return vty == CALL_INST_T;
    }

    BranchInst::BranchInst(LLVMContextPtr& context,
                        ValuePtr cond, ValuePtr trueBlock, ValuePtr falseBlock)
            : Instruction(context, context->VOID_TY, BRANCH_INST_T){
        createUse(cond);
        createUse(trueBlock);
        createUse(falseBlock);
    }

    void BranchInst::print(std::ostream& out) {
        out << "br ";
        getUsee(0)->printRefWithType(out);
        out << ", ";
        getUsee(1)->printRefWithType(out);
        out << ", ";
        getUsee(2)->printRefWithType(out);
    }

    BasicBlockPtr BranchInst::getTarget(bool cond) {
        int i = cond ? 1 : 0;
        auto t = getUsee(i + 1);
        assert(t->is(BASIC_BLOCK_T));
        auto targetBlock = std::static_pointer_cast<BasicBlock>(t);
        return targetBlock;
    }

    bool BranchInst::is(const ValueType vty) const {
        return vty == BRANCH_INST_T;
    }

    JumpInst::JumpInst(LLVMContextPtr& context, ValuePtr targetPtr)
            : Instruction(context, context->VOID_TY, JUMP_INST_T){
        createUse(targetPtr);
    }

    void JumpInst::print(std::ostream& out) {
        out << "br ";
        getUsee(0)->printRefWithType(out);
    }

    BasicBlockPtr JumpInst::getTarget() {
        auto t = getUsee(0);
        assert(t->is(BASIC_BLOCK_T));
        auto targetBlock = std::static_pointer_cast<BasicBlock>(t);
        return targetBlock;
    }

    bool JumpInst::is(const ValueType vty) const {
        return vty == JUMP_INST_T;
    }

    ReturnInst::ReturnInst(LLVMContextPtr& context, ValuePtr ret)
            : Instruction(context, context->VOID_TY, RETURN_INST_T){
        createUse(ret);
    }

    ReturnInst::ReturnInst(LLVMContextPtr& context)
            : Instruction(context, context->VOID_TY, RETURN_INST_T){
    }

    void ReturnInst::print(std::ostream & out) {
        out << "ret ";
        if (_usees.size() > 0) {
            getUsee(0)->printRefWithType(out);
        }
        else {
            _context->VOID_TY->print(out);
        }
    }

    bool ReturnInst::is(const ValueType vty) const {
        return vty == RETURN_INST_T;
    }

    PutInst::PutInst(LLVMContextPtr& context, ValuePtr value, PutInstID piid)
            : Instruction(context, context->VOID_TY, PUT_INST_T), _piid(piid) {
        createUse(value);
    }

    void PutInst::print(std::ostream &out) {
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

    bool PutInst::is(const ValueType vty) const {
        return vty == PUT_INST_T;
    }

    GetintInst::GetintInst(LLVMContextPtr context)
            : Instruction(context, context->I32_TY, GETINT_INST_T) { }

    void GetintInst::print(std::ostream& out) {
        printRef(out);
        out << " = ";
        out << "call ";
        _type->print(out);
        out << "@getint()";
    }

    bool GetintInst::is(const ValueType vty) const {
        return vty == GETINT_INST_T;
    }

    GetcharInst::GetcharInst(LLVMContextPtr context)
            : Instruction(context, context->I32_TY, GETCHAR_INST_T) { }

    void GetcharInst::print(std::ostream& out) {
        printRef(out);
        out << " = ";
        out << "call ";
        _type->print(out);
        out << "@getchar()";
    }

    bool GetcharInst::is(const ValueType vty) const {
        return vty == GETCHAR_INST_T;
    }

}
