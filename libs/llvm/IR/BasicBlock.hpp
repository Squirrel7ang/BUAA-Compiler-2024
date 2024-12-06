//
// Created by tang on 11/6/24.
//

#ifndef BASICBLOCK_HPP
#define BASICBLOCK_HPP

#include <ostream>
#include <set>

#include "Common.hpp"
#include "LLVMContext.hpp"
#include "Instructions.hpp"

namespace llvm {
    class BasicBlock : public Value {
        vector<InstructionPtr> _insts;
        vector<BasicBlockPtr> _preds;
        vector<BasicBlockPtr> _succs;
        std::set<InstructionPtr> _in; // data flow analyze
        std::set<InstructionPtr> _out; // data flow analyze
        std::set<InstructionPtr> _def; // data flow analyze
        std::set<InstructionPtr> _use; // data flow analyze
        int _spaceUse;
    protected:
        void addVarUse(InstructionPtr vp);
        void addVarDef(InstructionPtr vp);
    public:

        // iterator for instructions inside;
        vector<InstructionPtr>::iterator instructionBegin() {
            return _insts.begin();
        }
        vector<InstructionPtr>::iterator instructionEnd() {
            return _insts.end();
        }
        explicit BasicBlock(LLVMContextPtr& context);
        void addPred(const BasicBlockPtr & block);
        void addSucc(const BasicBlockPtr & block);
        void addInst(s_ptr<BasicBlock> me, InstructionPtr inst);
        bool addVarIn(InstructionPtr vp);
        bool addVarIn(const std::set<InstructionPtr>& vps);
        bool addVarOut(InstructionPtr vp);
        bool addVarOut(const std::set<InstructionPtr>& vps);
        const std::set<InstructionPtr>& getVarIn();
        const std::set<InstructionPtr>& getVarOut();
        const std::set<InstructionPtr>& getVarDef();
        const std::set<InstructionPtr>& getVarUse();
        void print(std::ostream& out) override;
        bool isEmptyBlock();
        void setIndex(int &index) override;
        void calUseDef();
        bool calVarIn();
        bool calVarOut();
        bool endWithReturn();
        int calSpaceUse();
    };
}

#endif //BASICBLOCK_HPP
