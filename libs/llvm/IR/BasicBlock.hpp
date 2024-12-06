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
        std::set<ValuePtr> _in; // data flow analyze
        std::set<ValuePtr> _out; // data flow analyze
        std::set<ValuePtr> _def; // data flow analyze
        std::set<ValuePtr> _use; // data flow analyze
        int _spaceUse;
    protected:
        void addVarUse(ValuePtr vp);
        void addVarDef(ValuePtr vp);
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
        bool addVarIn(ValuePtr vp);
        bool addVarIn(const std::set<ValuePtr>& vps);
        bool addVarOut(ValuePtr vp);
        bool addVarOut(const std::set<ValuePtr>& vps);
        const std::set<ValuePtr>& getVarIn();
        const std::set<ValuePtr>& getVarOut();
        const std::set<ValuePtr>& getVarDef();
        const std::set<ValuePtr>& getVarUse();
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
