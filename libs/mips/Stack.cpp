//
// Created by tang on 11/26/24.
//

#include "Stack.hpp"

namespace mips {
    StackSlotPtr StackSlot::New(unsigned int size) {
        return std::make_shared<StackSlot>(size);
    }

    void Stack::pushSlots(unsigned int bytes) {
        auto newSlot = StackSlot::New(bytes);
        _slots.push_back(newSlot);
    }

}
