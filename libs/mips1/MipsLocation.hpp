//
// Created by tang on 12/19/24.
//

#ifndef MIPSLOCATION_HPP
#define MIPSLOCATION_HPP
#include "MipsCommon.hpp"

namespace mips {

    class MipsLocation {
    public:
        virtual ~MipsLocation() = default;
        virtual MipsInstPtr writeBackReg(MipsRegPtr reg);
    };

    class Stack {
        std::vector<StackSlotPtr> _slots;
        unsigned int _size;
    public:
        explicit Stack();
        static StackPtr New();
        StackSlotPtr allocateSlot(unsigned int size);
        unsigned int getSize() { return _size; }
    };

    class StackSlot: public MipsLocation {
        const unsigned int _bytes;
        const unsigned int _pos;
        VariablePtr var; // this should always be $sp
    public:
        static StackSlotPtr New(unsigned int size, unsigned int pos);
        explicit StackSlot(unsigned int size, unsigned int pos);
        unsigned int getSize() { return _bytes; }
        unsigned int getOffset() { return _pos; }
        MipsInstPtr writeBackReg(MipsRegPtr) override;
    };

    class StaticSeg: public MipsLocation {
    public:

    };

}

#endif //MIPSLOCATION_HPP
