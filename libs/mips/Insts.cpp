//
// Created by tang on 11/26/24.
//

#include "Insts.hpp"
#include "MipsImm.hpp"
#include "Regs.hpp"

namespace mips {
    MipsInstType MipsInst::idToType(MipsInstID id) {
        if (MIID_J <= id && id <= MIID_JALR) { return MIT_J; }
        else if (MIID_ADDI <= id && id <= MIID_BLT) { return MIT_I; }
        else if (MIID_ADD <= id && id <= MIID_JR) { return MIT_R; }
        else assert(0);
    }

    void MipsInst::print(std::ostream &out) {
        assert(0);
    }

    /******************** R-INST ********************/

    RInst::RInst(const MipsRegPtr& rs,
                 const MipsRegPtr& rt,
                 const MipsRegPtr& rd,
                 MipsInstID instID)
        : MipsInst(instID), _rs(rs), _rt(rt), _rd(rd), _sa(IMM_ZERO) {
        assert(MipsInst::idToType(instID) == MIT_R);
    }

    RInstPtr RInst::New(const MipsRegPtr &rs,
                        const MipsRegPtr &rt,
                        const MipsRegPtr &rd,
                        MipsInstID instID) {
        return std::make_shared<RInst>(rs, rt, rd, instID);
    }

    void RInst::print(std::ostream& out) override {
        switch (_id) {
            case MIID_ADD: out << "add"; goto print0;
            case MIID_SUB: out << "sub"; goto print0;
            case MIID_MULT: out << "mult"; goto print0;
            case MIID_DIV: out << "div"; goto print0;
            case MIID_AND: out << "and"; goto print0;
            case MIID_OR: out << "or"; goto print0;
            case MIID_XOR: out << "xor"; goto print0;

            case MIID_SLL: out << "sll"; goto print1;
            case MIID_SRA: out << "sra"; goto print1;
            case MIID_SRL: out << "srl"; goto print1;

            case MIID_MFHI: out << "mfhi"; goto print2;
            case MIID_MFLO: out << "mflo"; goto print2;

            case MIID_MTHI: out << "mthi"; goto print3;
            case MIID_MTLO: out << "mtlo"; goto print3;

            case MIID_JR: out << "jr"; goto print3;
            default: assert(0);
        }
    print0:
        // add  sub  mult  div  and  or  xor
        out << " ";
        _rd->print(out);
        out << ", ";
        _rs->print(out);
        out << ", ";
        _rt->print(out);
        return;
    print1:
        // sll  sra  srl
        out << " ";
        _rd->print(out);
        out << ", ";
        _rt->print(out);
        out << ", ";
        _sa->print(out);
        return;
    print2:
        // mfhi  mflo
        out << " ";
        _rd->print(out);
        return;
    print3:
        // mthi  mtlo  jr
        out << " ";
        _rs->print(out);
    }

    /******************** I-INST ********************/
    IInst::IInst(const MipsRegPtr& rs,
                 const MipsRegPtr& rt,
                 const MipsDataPtr& imm,
                 MipsInstID instID)
        : MipsInst(instID), _rs(rs), _rt(rt), _imm(imm) { }

    IInstPtr IInst::New(const MipsRegPtr &rs,
                        const MipsRegPtr &rt,
                        const MipsDataPtr &imm,
                        MipsInstID instID) {
        return std::make_shared<IInst>(rs, rt, imm, instID);
    }

    /******************** J-INST ********************/
    JInst::JInst(const MipsDataPtr imm, MipsInstID instID)
        : MipsInst(instID), _imm(imm) { }

    JInstPtr JInst::New(const MipsDataPtr& imm, MipsInstID instID) {
        return std::make_shared<JInst>(imm, instID);
    }
}
