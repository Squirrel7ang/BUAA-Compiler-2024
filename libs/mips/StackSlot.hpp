//
// Created by tang on 11/26/24.
//

#ifndef STACKSLOT_HPP
#define STACKSLOT_HPP

#include <vector>

#include "MipsCommon.hpp"
#include "VarLocation.hpp"

namespace mips {
    class Stack {
        std::vector<StackSlotPtr> _slots;
        unsigned int _size;
    public:
        explicit Stack();
        static StackPtr New();
        StackSlotPtr allocateSlot(unsigned int size);
        unsigned int getSize() { return _size; }
    };

    class StackSlot : public VarLocation {
        const unsigned int _bytes;
        const unsigned int _pos;
        VariablePtr var; // this should always be $sp
    public:
        static StackSlotPtr New(unsigned int size, unsigned int pos);
        explicit StackSlot(unsigned int size, unsigned int pos);
        unsigned int getSize() { return _bytes; }
        unsigned int getOffset() { return _pos; }
    };
}

#endif //STACKSLOT_HPP

