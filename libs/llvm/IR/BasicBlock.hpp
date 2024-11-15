//
// Created by tang on 11/6/24.
//

#ifndef BASICBLOCK_HPP
#define BASICBLOCK_HPP

#include "Common.hpp"

namespace llvm {
    class BasicBlock : public Value {
        vector<InstructionPtr> insts;
    public:
        explicit BasicBlock(LLVMContextPtr context)
                : Value(context, context->LABEL_TY, BASIC_BLOCK_T) { }
        void addInst(InstructionPtr inst) {
            insts.push_back(inst);
        }
    };
}

#endif //BASICBLOCK_HPP
