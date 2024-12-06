//
// Created by tang on 11/25/24.
//

#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include <ostream>

#include "MipsCommon.hpp"
#include "VarLocation.hpp"

namespace mips {
    enum MipsRegType {
        MRT_ZERO, // zero
        MRT_TMP, // t0-t7, t8-t9
        MRT_SAVE, // s0-s7
        MRT_ARG, // a0-a3
        MRT_SP, // sp
        MRT_RETVAL, // v0, v1
        MRT_RA, // ra

        MRT_HI,
        MRT_LO,
    };

    class MipsReg : public VarLocation {
    private:
        int _regNum;
        MipsRegType _type;
    public:
        static MipsRegPtr New(int regNum);
        bool operator==(const MipsReg & other) const;
        void print(std::ostream& out) const;
        static MipsRegType regNumToID(unsigned int n);
        MipsRegType getType() const { return _type; }
    private:
        explicit MipsReg(int regNum);
    };
}

#endif //REGISTERS_HPP
