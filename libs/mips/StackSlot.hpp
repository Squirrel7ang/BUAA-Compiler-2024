//
// Created by tang on 11/26/24.
//

#ifndef STACKSLOT_HPP
#define STACKSLOT_HPP

#include <vector>

#include "MipsCommon.hpp"
#include "VarLocation.hpp"

namespace mips {
    class StackSlot : public VarLocation {
        const unsigned int _bytes;
        VariablePtr var; // this should always be $sp
    private:
        explicit StackSlot(unsigned int size);
    public:
        static StackSlotPtr New(unsigned int size);
        unsigned int getSize() { return _bytes; }
    };
}

#endif //STACKSLOT_HPP

