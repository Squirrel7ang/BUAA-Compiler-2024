//
// Created by tang on 11/26/24.
//

#ifndef STACKFRAME_HPP
#define STACKFRAME_HPP

#include <vector>

#include "MipsCommon.hpp"

namespace mips {
    class StackSlot {
        const unsigned int _bytes;
        VariablePtr var; // this should always be $sp
    private:
        explicit StackSlot(unsigned int size);
    public:
        static StackSlotPtr New(unsigned int size);
        unsigned int getSize() { return _bytes; }
    };

    class Stack {
        std::vector<StackSlotPtr> _slots;
        unsigned int _bytes;
    public:
        void pushSlots(unsigned int bytes);
    };
}

#endif //STACKFRAME_HPP
