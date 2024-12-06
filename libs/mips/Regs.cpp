//
// Created by tang on 11/26/24.
//

#include <cassert>

#include "Regs.hpp"

namespace mips {
    MipsRegPtr MipsReg::New(int regNum) {
        return std::make_shared<MipsReg>(regNum);
    }

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
            case(26): assert(0);
            case(27): assert(0);
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
            case(25): return MRT_TMP;
            case(26): assert(0); // k0
            case(27): assert(0); // k1
            case(28): assert(0); // gp
            case(29): return MRT_SP;
            case(30): assert(0); // fp
            case(31): return MRT_RA;
            default: assert(0);
        }
    }
}
