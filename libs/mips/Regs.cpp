//
// Created by tang on 11/26/24.
//

#include <cassert>

#include "Regs.hpp"

namespace mips {
    MipsRegPtr MipsReg::New(int regNum) {
        return std::make_shared<MipsReg>(regNum);
    }

    bool MipsReg::operator==(const MipsReg &other) const {
        return other._regNum == this->_regNum;
    }

    MipsReg::MipsReg(int regNum) : VarLocation(VLID_REG), _regNum(regNum), _type(regNumToID(regNum)) { }

    void MipsReg::print(std::ostream& out) const {
        switch (_regNum) {
            case(0): out << "$zero"; break;
            case(1): assert(0);
            case(2): out << "$v0"; break;
            case(3): out << "$v1"; break;
            case(4): out << "$a0"; break;
            case(5): out << "$a1"; break;
            case(6): out << "$a2"; break;
            case(7): out << "$a3"; break;
            case(8): out << "$t0"; break;
            case(9): out << "$t1"; break;
            case(10): out << "$t2"; break;
            case(11): out << "$t3"; break;
            case(12): out << "$t4"; break;
            case(13): out << "$t5"; break;
            case(14): out << "$t6"; break;
            case(15): out << "$t7"; break;
            case(16): out << "$s0"; break;
            case(17): out << "$s1"; break;
            case(18): out << "$s2"; break;
            case(19): out << "$s3"; break;
            case(20): out << "$s4"; break;
            case(21): out << "$s5"; break;
            case(22): out << "$s6"; break;
            case(23): out << "$s7"; break;
            case(24): out << "$t8"; break;
            case(25): out << "$t9"; break;
            case(26): out << "$k0"; break;
            case(27): out << "$k1"; break;
            case(28): assert(0);
            case(29): out << "$sp"; break;
            case(30): assert(0);
            case(31): out << "$ra"; break;
            default: assert(0);
        }
    }

    MipsRegType MipsReg::regNumToID(unsigned int n) {
        switch (n) {
            case(0): return MRT_ZERO;
            case(1): assert(0); // at
            case(2): /* Fall through*/
            case(3): return MRT_RETVAL;
            case(4): /* Fall through*/
            case(5): /* Fall through*/
            case(6): /* Fall through*/
            case(7): return MRT_ARG;
            case(8): /* Fall through*/
            case(9): /* Fall through*/
            case(10): /* Fall through*/
            case(11): /* Fall through*/
            case(12): /* Fall through*/
            case(13): /* Fall through*/
            case(14): /* Fall through*/
            case(15): return MRT_TMP;
            case(16): /* Fall through*/
            case(17): /* Fall through*/
            case(18): /* Fall through*/
            case(19): /* Fall through*/
            case(20): /* Fall through*/
            case(21): /* Fall through*/
            case(22): /* Fall through*/
            case(23): return MRT_SAVE;
            case(24): /* Fall through*/
            case(25): /* Fall through*/
            case(26): /* Fall through*/
            case(27): return MRT_TMP;
            case(28): assert(0); // gp
            case(29): return MRT_SP;
            case(30): assert(0); // fp
            case(31): return MRT_RA;
            default: assert(0);
        }
    }

    // extern MipsRegPtr REG_ZERO = MipsReg::New(0);
    // // extern const MipsRegPtr REG_AT = MipsReg:New(1);
    // extern const MipsRegPtr REG_V0 = MipsReg::New(2);
    // extern const MipsRegPtr REG_V1 = MipsReg::New(3);
    // extern const MipsRegPtr REG_A0 = MipsReg::New(4);
    // extern const MipsRegPtr REG_A1 = MipsReg::New(5);
    // extern const MipsRegPtr REG_A2 = MipsReg::New(6);
    // extern const MipsRegPtr REG_A3 = MipsReg::New(7);
    // extern const MipsRegPtr REG_T0 = MipsReg::New(8);
    // extern const MipsRegPtr REG_T1 = MipsReg::New(9);
    // extern const MipsRegPtr REG_T2 = MipsReg::New(10);
    // extern const MipsRegPtr REG_T3 = MipsReg::New(11);
    // extern const MipsRegPtr REG_T4 = MipsReg::New(12);
    // extern const MipsRegPtr REG_T5 = MipsReg::New(13);
    // extern const MipsRegPtr REG_T6 = MipsReg::New(14);
    // extern const MipsRegPtr REG_T7 = MipsReg::New(15);
    // extern const MipsRegPtr REG_S0 = MipsReg::New(16);
    // extern const MipsRegPtr REG_S1 = MipsReg::New(17);
    // extern const MipsRegPtr REG_S2 = MipsReg::New(18);
    // extern const MipsRegPtr REG_S3 = MipsReg::New(19);
    // extern const MipsRegPtr REG_S4 = MipsReg::New(20);
    // extern const MipsRegPtr REG_S5 = MipsReg::New(21);
    // extern const MipsRegPtr REG_S6 = MipsReg::New(22);
    // extern const MipsRegPtr REG_S7 = MipsReg::New(23);
    // extern const MipsRegPtr REG_T8 = MipsReg::New(24);
    // extern const MipsRegPtr REG_T9 = MipsReg::New(25);
    // extern const MipsRegPtr REG_K0 = MipsReg::New(26);
    // extern const MipsRegPtr REG_K1 = MipsReg::New(27);
    // // extern const MipsRegPtr REG_GP = MipsReg::New(28);
    // extern const MipsRegPtr REG_SP = MipsReg::New(29);
    // // extern const MipsRegPtr REG_FP = MipsReg::New(30);
    // extern const MipsRegPtr REG_RA = MipsReg::New(31);
}
