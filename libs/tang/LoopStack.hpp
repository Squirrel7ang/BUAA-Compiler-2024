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
        llvm::BasicBlockPtr bodyBlock;
        llvm::BasicBlockPtr outerBlock;
    public:
        explicit Loop(llvm::BasicBlockPtr body, llvm::BasicBlockPtr outer)
                : bodyBlock(body), outerBlock(outer) { }
        llvm::BasicBlockPtr getBody() { return bodyBlock; }
        llvm::BasicBlockPtr getOuter() { return outerBlock; }
    };

    class LoopStack {
        vector<Loop> loops;
        ErrorReporter& _reporter;
    public:
        explicit LoopStack(ErrorReporter& reporter)
            : _reporter(reporter) { }
        void pushLoop(llvm::BasicBlockPtr body, llvm::BasicBlockPtr outer) {
            auto l = Loop(body, outer);
            loops.push_back(l);
        }
        void popLoop() { loops.pop_back(); }
        bool checkBreakContinue(const unsigned int lin) {
            if (loops.empty()) {
                _reporter.report(lin, 'm');
            }
            return !loops.empty();
        }
        llvm::BasicBlockPtr getCurrentLoopBody() {
            return loops.back().getBody();
        }
        llvm::BasicBlockPtr getCurrentLoopOuter() {
            return loops.back().getOuter();
        }
    };
} // namespace tang

#endif //LOOPSTACK_HPP