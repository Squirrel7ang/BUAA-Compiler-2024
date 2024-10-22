//
// Created by tang on 10/20/24.
//

#ifndef LOOPSTACK_HPP
#define LOOPSTACK_HPP
#include <vector>

#include "ast.hpp"

namespace tang {

    class Loop {

    };

    class LoopStack {
        vector<Loop> loops;
    public:
        void pushLoop() {
            Loop l = Loop();
            loops.push_back(l);
        }
        void popLoop() {
            Loop l = Loop();
            loops.push_back(l);
        }
        bool checkReturnContinue() {
            return !loops.empty();
        }
    };
} // namespace tang

#endif //LOOPSTACK_HPP