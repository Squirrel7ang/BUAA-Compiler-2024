//
// Created by tang on 11/25/24.
//

#ifndef MIPSREGISTERS_HPP
#define MIPSREGISTERS_HPP

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
        unsigned int _regNum;
        MipsRegType _type;
    public:
        explicit MipsReg(unsigned int regNum);
        explicit MipsReg(unsigned int regNum, int offset);
        static MipsRegPtr New(unsigned int regNum);
        static MipsRegPtr New(unsigned int regNum, int offset);
        bool operator==(const MipsReg & other) const;
        bool operator!=(const MipsReg & other) const;
        void print(std::ostream& out) const;
        unsigned int getRegNum();
        static MipsRegType regNumToID(unsigned int n);
        MipsRegType getType() const { return _type; }
    };

    static const MipsRegPtr REG_ZERO = MipsReg::New(0);
    // static const MipsRegPtr REG_AT = MipsReg:New(1);
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
    static const MipsRegPtr REG_K0 = MipsReg::New(26);
    static const MipsRegPtr REG_K1 = MipsReg::New(27);
    // static const MipsRegPtr REG_GP = MipsReg::New(28);
    static const MipsRegPtr REG_SP = MipsReg::New(29);
    // static const MipsRegPtr REG_FP = MipsReg::New(30);
    static const MipsRegPtr REG_RA = MipsReg::New(31);

    const int REG_ZERO_NUM = 0;
    // const int REG_AT_NUM = 1;
    const int REG_V0_NUM = 2;
    const int REG_V1_NUM = 3;
    const int REG_A0_NUM = 4;
    const int REG_A1_NUM = 5;
    const int REG_A2_NUM = 6;
    const int REG_A3_NUM = 7;
    const int REG_T0_NUM = 8;
    const int REG_T1_NUM = 9;
    const int REG_T2_NUM = 10;
    const int REG_T3_NUM = 11;
    const int REG_T4_NUM = 12;
    const int REG_T5_NUM = 13;
    const int REG_T6_NUM = 14;
    const int REG_T7_NUM = 15;
    const int REG_S0_NUM = 16;
    const int REG_S1_NUM = 17;
    const int REG_S2_NUM = 18;
    const int REG_S3_NUM = 19;
    const int REG_S4_NUM = 20;
    const int REG_S5_NUM = 21;
    const int REG_S6_NUM = 22;
    const int REG_S7_NUM = 23;
    const int REG_T8_NUM = 24;
    const int REG_T9_NUM = 25;
    const int REG_K0_NUM = 26;
    const int REG_K1_NUM = 27;
    // const int REG_GP_NUM = 28;
    const int REG_SP_NUM = 29;
    // const int REG_FP_NUM = 30;
    const int REG_RA_NUM = 31;
}

#endif // MIPSREGISTERS_HPP
