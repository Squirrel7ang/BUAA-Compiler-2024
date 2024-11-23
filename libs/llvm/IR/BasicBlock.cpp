//
// Created by tang on 11/6/24.
//

#include "BasicBlock.hpp"

namespace llvm {
    BasicBlock::BasicBlock(LLVMContextPtr& context)
            : Value(context, context->LABEL_TY, BASIC_BLOCK_T) { }

    void BasicBlock::addInst(InstructionPtr inst) {
        insts.push_back(inst);
    }

    void BasicBlock::print(std::ostream& out) {
        out << _index << ':';
        out << std::endl;
        for (auto& inst: insts) {
            out << "  ";
            inst->print(out);
            out << std::endl;
        }
    }

    bool BasicBlock::isEmptyBlock() { return insts.empty(); }

    void BasicBlock::setIndex(int &index) {
        _index = index;
        index++;
        for (auto& inst: insts) {
            if (!inst->getType()->isVoidTy()) {
                inst->setIndex(index);
            }
        }
    }

}