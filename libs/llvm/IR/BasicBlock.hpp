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
    public:
        explicit BasicBlock(LLVMContextPtr& context);
        void addPred(const BasicBlockPtr & block);
        void addSucc(const BasicBlockPtr & block);
        void addInst(InstructionPtr inst);
        void print(std::ostream& out) override;
        bool isEmptyBlock();
        void setIndex(int &index) override;

    };
}

#endif //BASICBLOCK_HPP
