//
// Created by tang on 10/20/24.
//

#ifndef LOOPSTACK_HPP
#define LOOPSTACK_HPP
#include <vector>

#include "ast.hpp"
#include "error.hpp"

namespace tang {

    class Loop {

    };

    class LoopStack {
        vector<Loop> loops;
        ErrorReporter& _reporter;
    public:
        explicit LoopStack(ErrorReporter& reporter)
            : _reporter(reporter) { }
        void pushLoop() {
            auto l = Loop();
            loops.push_back(l);
        }
        void popLoop() {
            auto l = Loop();
            loops.push_back(l);
        }
        bool checkBreakContinue(const unsigned int lin) {
            if (loops.empty()) {
                _reporter.report(lin, 'm');
            }
            return !loops.empty();
        }
    };
} // namespace tang

#endif //LOOPSTACK_HPP