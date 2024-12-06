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

    static const MipsRegPtr REG_ZERO = MipsReg::New(0);
    // static const MipsReg REG_AT = MipsReg:New(1);
    static const MipsRegPtr REG_V0 = MipsReg::New(2);
    static const MipsRegPtr REG_V1 = MipsReg::New(3);
    static const MipsRegPtr REG_A0 = MipsReg::New(4);
    static const MipsRegPtr REG_A1 = MipsReg::New(5);
    static const MipsRegPtr REG_A2 = MipsReg::New(6);
    static const MipsRegPtr REG_A3 = MipsReg::New(7);
    static const MipsRegPtr REG_T0 = MipsReg::New(8);
    static const MipsRegPtr REG_T1 = MipsReg::New(9);
    static const MipsRegPtr REG_T2 = MipsReg::New(10);
    static const MipsRegPtr REG_T3 = MipsReg::New(11);
    static const MipsRegPtr REG_T4 = MipsReg::New(12);
    static const MipsRegPtr REG_T5 = MipsReg::New(13);
    static const MipsRegPtr REG_T6 = MipsReg::New(14);
    static const MipsRegPtr REG_T7 = MipsReg::New(15);
    static const MipsRegPtr REG_S0 = MipsReg::New(16);
    static const MipsRegPtr REG_S1 = MipsReg::New(17);
    static const MipsRegPtr REG_S2 = MipsReg::New(18);
    static const MipsRegPtr REG_S3 = MipsReg::New(19);
    static const MipsRegPtr REG_S4 = MipsReg::New(20);
    static const MipsRegPtr REG_S5 = MipsReg::New(21);
    static const MipsRegPtr REG_S6 = MipsReg::New(22);
    static const MipsRegPtr REG_S7 = MipsReg::New(23);
    static const MipsRegPtr REG_T8 = MipsReg::New(24);
    static const MipsRegPtr REG_T9 = MipsReg::New(25);
    // static const MipsReg REG_K0 = MipsReg::New(26);
    // static const MipsReg REG_K1 = MipsReg::New(27);
    // static const MipsReg REG_GP = MipsReg::New(28);
    static const MipsRegPtr REG_SP = MipsReg::New(29);
    // static const MipsReg REG_FP = MipsReg::New(30);
    static const MipsRegPtr REG_RA = MipsReg::New(31);
}

#endif //REGISTERS_HPP
