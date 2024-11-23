//
// Created by tang on 11/6/24.
//

#ifndef BASICBLOCK_HPP
#define BASICBLOCK_HPP

#include <ostream>

#include "Common.hpp"
#include "LLVMContext.hpp"
#include "Instructions.hpp"

namespace llvm {
    class BasicBlock : public Value {
        vector<InstructionPtr> insts;
    public:
        explicit BasicBlock(LLVMContextPtr& context);
        void addInst(InstructionPtr inst);
        void print(std::ostream& out);
        bool isEmptyBlock();
        void setIndex(int &index) override;

    };
}

#endif //BASICBLOCK_HPP
