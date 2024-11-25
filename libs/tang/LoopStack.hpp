//
// Created by tang on 10/20/24.
//

#ifndef LOOPSTACK_HPP
#define LOOPSTACK_HPP
#include <vector>

#include "IR/Common.hpp"
#include "Ast.hpp"
#include "ErrorReporter.hpp"

namespace tang {

    class Loop {
        llvm::BasicBlockPtr condBlock;
        llvm::BasicBlockPtr bodyBlock;
        llvm::BasicBlockPtr updateBlock;
        llvm::BasicBlockPtr outerBlock;
    public:
        explicit Loop(llvm::BasicBlockPtr cond,
                      llvm::BasicBlockPtr body,
                      llvm::BasicBlockPtr update,
                      llvm::BasicBlockPtr outer)
                : condBlock(cond), bodyBlock(body),
                  outerBlock(outer), updateBlock(update) { }

        llvm::BasicBlockPtr getCond() { return condBlock; }
        llvm::BasicBlockPtr getBody() { return bodyBlock; }
        llvm::BasicBlockPtr getOuter() { return outerBlock; }
        llvm::BasicBlockPtr getUpdate() { return updateBlock; }
    };

    class LoopStack {
        vector<Loop> loops;
        ErrorReporter& _reporter;
    public:
        explicit LoopStack(ErrorReporter& reporter)
            : _reporter(reporter) { }
        void pushLoop(llvm::BasicBlockPtr cond,
                      llvm::BasicBlockPtr body,
                      llvm::BasicBlockPtr update,
                      llvm::BasicBlockPtr outer) {
            auto&& l = Loop(cond, body, update, outer);
            loops.push_back(l);
        }
        void popLoop() { loops.pop_back(); }
        bool checkBreakContinue(const unsigned int lin) {
            if (loops.empty()) {
                _reporter.report(lin, 'm');
            }
            return !loops.empty();
        }
        llvm::BasicBlockPtr getCurrentLoopCond() { return loops.back().getCond(); }
        llvm::BasicBlockPtr getCurrentLoopBody() { return loops.back().getBody(); }
        llvm::BasicBlockPtr getCurrentLoopOuter() { return loops.back().getOuter(); }
        llvm::BasicBlockPtr getCurrentLoopUpdate() { return loops.back().getUpdate(); }
    };
} // namespace tang

#endif //LOOPSTACK_HPP