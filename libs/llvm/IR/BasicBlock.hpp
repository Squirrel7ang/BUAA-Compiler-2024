//
// Created by tang on 11/6/24.
//

#ifndef BASICBLOCK_HPP
#define BASICBLOCK_HPP

#include "Common.hpp"

namespace llvm {
    class BasicBlock {
        vector<InstructionPtr> insts;
    public:
        void addInst(InstructionPtr inst) {
            insts.push_back(inst);
        }
    };
}

#endif //BASICBLOCK_HPP
