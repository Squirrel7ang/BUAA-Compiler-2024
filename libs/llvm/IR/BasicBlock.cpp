//
// Created by tang on 11/6/24.
//

#include <algorithm>
#include "BasicBlock.hpp"

namespace llvm {
    void BasicBlock::addVarUse(InstructionPtr vp) {
        if (!_def.contains(vp)) {
            _use.insert(vp);
        }
    }

    void BasicBlock::addVarDef(InstructionPtr vp) {
        if (!_use.contains(vp)) {
            _def.insert(vp);
        }
    }

    BasicBlock::BasicBlock(LLVMContextPtr& context)
            : Value(context, context->LABEL_TY, BASIC_BLOCK_T),
              _spaceUse(0) { }

    void BasicBlock::addPred(const BasicBlockPtr &block) {
        _preds.push_back(block);
    }

    void BasicBlock::addSucc(const BasicBlockPtr &block) {
        _succs.push_back(block);
    }

    const std::set<InstructionPtr>& BasicBlock::getVarIn() {
        return _in;
    }

    const std::set<InstructionPtr>& BasicBlock::getVarOut() {
        return _out;
    }

    const std::set<InstructionPtr>& BasicBlock::getVarDef() {
        return _def;
    }

    const std::set<InstructionPtr>& BasicBlock::getVarUse() {
        return _use;
    }

    void BasicBlock::addInst(const s_ptr<BasicBlock> me, InstructionPtr inst) {
        _insts.push_back(inst);

        if (inst->is(JUMP_INST_T)) {
            JumpInstPtr jip = std::static_pointer_cast<JumpInst>(inst);
            auto target = jip->getTarget();
            target->addPred(me);
            this->addSucc(target);
        }
        else if (inst->is(BRANCH_INST_T)) {
            BranchInstPtr bip = std::static_pointer_cast<BranchInst>(inst);
            auto target0 = bip->getTarget(true);
            auto target1 = bip->getTarget(false);
            this->addSucc(target0);
            target0->addPred(me);
            this->addSucc(target1);
            target1->addPred(me);
        }
        else if (inst->is(ALLOCA_INST_T)) {
            AllocaInstPtr aip = std::static_pointer_cast<AllocaInst>(inst);
            _spaceUse += aip->allocateSpace();
        }
    }

    bool BasicBlock::addVarIn(InstructionPtr vp) {
        bool changed = false;
        if (!_in.contains(vp)) {
            _in.insert(vp);
            changed = true;
        }
        return changed;
    }

    bool BasicBlock::addVarIn(const std::set<InstructionPtr> &vps) {
        bool changed = false;
        for (auto& vp: vps) {
            if (_in.contains(vp))
                changed = true;
            else
                _in.insert(vp);
        }
        return changed;
    }

    bool BasicBlock::addVarOut(InstructionPtr vp) {
        bool changed = false;
        if (!_out.contains(vp)) {
            _out.insert(vp);
            changed = true;
        }
        return changed;
    }

    bool BasicBlock::addVarOut(const std::set<InstructionPtr> &vps) {
        bool changed = false;
        for (auto& vp: vps) {
            if (_out.contains(vp))
                changed = true;
            else
                _out.insert(vp);
        }
        return changed;
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

    void BasicBlock::calUseDef() {
        _def.clear();
        _use.clear();
        for (auto& inst: _insts) {
            // add use first, then def
            for (int i = 0; i < inst->getUseeSize(); i++) {
                auto usee = inst->getUsee(i);
                if (inst->getUsee(i)->isInst()) {
                    auto&& instUsee = std::static_pointer_cast<Instruction>(inst->getUsee(i));
                    addVarUse(instUsee);
                }
            }
            if (!inst->getType()->equals(_context->VOID_TY)) {
                addVarDef(inst);
            }
        }
    }

    bool BasicBlock::calVarOut() {
        int changed = false;
        for (auto& block: _succs) {
            changed |= addVarOut(block->getVarIn());
        }
        return changed;
    }

    bool BasicBlock::endWithReturn() {
        return _insts.back()->is(RETURN_INST_T);
    }

    int BasicBlock::calSpaceUse() {
        return _spaceUse;
    }

    bool BasicBlock::calVarIn() {
        bool changed = false;

        std::set<InstructionPtr> outCutDef;
        std::set_difference(_out.begin(), _out.end(), _def.begin(), _def.end(),
            std::inserter(outCutDef, outCutDef.begin()));

        changed |= addVarIn(_use);
        changed |= addVarIn(outCutDef);

        return changed;
    }
}
