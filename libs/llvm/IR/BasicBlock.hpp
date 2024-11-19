//
// Created by tang on 11/6/24.
//

#ifndef BASICBLOCK_HPP
#define BASICBLOCK_HPP

#include <ostream>

#include "Common.hpp"
#include "Instructions.hpp"

namespace llvm {
    class BasicBlock : public Value {
        vector<InstructionPtr> insts;
    public:
        explicit BasicBlock(LLVMContextPtr& context)
                : Value(context, context->LABEL_TY, BASIC_BLOCK_T) { }
        void addInst(InstructionPtr inst) {
            insts.push_back(inst);
        }
        void print(std::ostream& out) {
            // TODO: print tmpVar's Index
            out << std::endl;
            for (auto& inst: insts) {
                inst->print(out);
            }
        }
    };
}

#endif //BASICBLOCK_HPP
