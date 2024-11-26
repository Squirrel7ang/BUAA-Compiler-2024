//
// Created by tang on 11/6/24.
//

#include "BasicBlock.hpp"

namespace llvm {
    BasicBlock::BasicBlock(LLVMContextPtr& context)
            : Value(context, context->LABEL_TY, BASIC_BLOCK_T) { }

    void BasicBlock::addPred(const BasicBlockPtr &block) {
        _preds.push_back(block);
    }

    void BasicBlock::addSucc(const BasicBlockPtr &block) {
        _succs.push_back(block);
    }

    void BasicBlock::addInst(InstructionPtr inst) {
        _insts.push_back(inst);

        // TODO: add preds as well
        auto me = std::shared_ptr<BasicBlock>(this);
        if (inst->is(JUMP_INST_T)) {
            JumpInstPtr jip = std::static_pointer_cast<JumpInst>(inst);
            auto target = jip->getTarget();
            this->addSucc(target);
        }
        else if (inst->is(BRANCH_INST_T)) {
            BranchInstPtr bip = std::static_pointer_cast<BranchInst>(inst);
            auto target0 = bip->getTarget(true);
            auto target1 = bip->getTarget(false);
            this->addSucc(target0);
            this->addSucc(target1);
        }
    }

    void BasicBlock::print(std::ostream& out) {
        out << _index << ':';
        out << "                                                  ";
        out << "; preds = ";
        for (auto& pred: _preds) {
            pred->printRef(out);
            out << ", ";
        }
        out << std::endl;
        for (auto& inst: _insts) {
            out << "  ";
            inst->print(out);
            out << std::endl;
        }
    }

    bool BasicBlock::isEmptyBlock() { return _insts.empty(); }

    void BasicBlock::setIndex(int &index) {
        _index = index;
        index++;
        for (auto& inst: _insts) {
            if (!inst->getType()->isVoidTy()) {
                inst->setIndex(index);
            }
        }
    }

}