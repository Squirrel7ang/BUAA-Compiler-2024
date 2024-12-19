//
// Created by tang on 12/19/24.
//

#include "MipsLocation.hpp"

#include <cassert>

namespace mips {
    MipsInstPtr MipsLocation::writeBackReg(MipsRegPtr reg) {
        assert(0);
    }

    Stack::Stack(): _size(0) {
    }

    StackPtr Stack::New() {
        return std::make_shared<Stack>();
    }

    StackSlotPtr Stack::allocateSlot(unsigned int size) {
        auto slot = StackSlot::New(size, _size);
        _slots.push_back(slot);
        _size += size;
        return slot;
    }

    StackSlot::StackSlot(unsigned int size, unsigned int pos)
        : _bytes(size), _pos(pos) {
    }

    StackSlotPtr StackSlot::New(unsigned int size, unsigned int pos) {
        return std::make_shared<StackSlot>(size, pos);
    }
}
