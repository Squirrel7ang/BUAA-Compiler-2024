//
// Created by tang on 11/26/24.
//

#include "StackSlot.hpp"

namespace mips {
    StackSlot::StackSlot(unsigned int size)
        : VarLocation(VLID_STACK), _bytes(size){
    }

    StackSlotPtr StackSlot::New(unsigned int size) {
        return std::make_shared<StackSlot>(size);
    }
}
