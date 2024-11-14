//
// Created by tang on 11/7/24.
//

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "GlobalValue.hpp"
#include "Type.hpp"

namespace llvm {
    class Function : public GlobalValue {
    private:
        vector<BasicBlockPtr> blocks;
    public:
        explicit Function(LLVMContextPtr context, TypePtr ty)
                : GlobalValue(context, ty, FUNCTION_T) { }
        void addBasicBlock(BasicBlockPtr block) {
            blocks.push_back(block);
        }
    };
}

#endif //FUNCTION_HPP
