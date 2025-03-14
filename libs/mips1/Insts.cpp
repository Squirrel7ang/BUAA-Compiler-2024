//
// Created by tang on 11/26/24.
//

#include "Insts.hpp"
#include "MipsImm.hpp"
#include "MipsLabel.hpp"
#include "Regs.hpp"

namespace mips {
    MipsInstType MipsInst::idToType(MipsInstID id) {
        if (MIID_J <= id && id <= MIID_JAL) { return MIT_J; }
        else if (MIID_ADDI <= id && id <= MIID_LA) { return MIT_I; }
        else if (MIID_ADD <= id && id <= MIID_MOVE) { return MIT_R; }
        else assert(0);
    }

    void MipsInst::print(std::ostream &out) {
        assert(0);
    }

    /******************** R-INST ********************/

    RInst::RInst(const MipsRegPtr& rs,
                 const MipsRegPtr& rt,
                 const MipsRegPtr& rd,
                 const MipsImmPtr& imm,
                 MipsInstID instID)
        : MipsInst(instID), _rs(rs), _rt(rt), _rd(rd), _sa(imm) {
        assert(MipsInst::idToType(instID) == MIT_R);
    }

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
                        const MipsImmPtr &imm,
                        MipsInstID instID) {
        return std::make_shared<RInst>(rs, rt, rd, imm, instID);
    }

    RInstPtr RInst::New(const MipsRegPtr &rs,
                        const MipsRegPtr &rt,
                        const MipsRegPtr &rd,
                        MipsInstID instID) {
        return std::make_shared<RInst>(rs, rt, rd, instID);
    }

    RInstPtr RInst::NewSyscall() {
        return RInst::New(REG_ZERO, REG_ZERO, REG_ZERO, MIID_SYSCALL);
    }

    RInstPtr RInst::NewJr() {
        return RInst::New(REG_RA, REG_ZERO, REG_ZERO, MIID_JR);
    }

    RInstPtr RInst::NewMove(const MipsRegPtr &rt, const MipsRegPtr &rd) {
        return RInst::New(REG_ZERO, rt, rd, MIID_MOVE);
    }

    void RInst::print(std::ostream& out) {
        if (_label != nullptr) {
            _label->print(out);
        }
        out << "  ";
        switch (_id) {
            case MIID_ADD: out << "add"; goto print0;
            case MIID_SUB: out << "sub"; goto print0;
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

            case MIID_MOVE: out << "move"; goto print4;

            case MIID_MULT: out << "mult"; goto print5;
            case MIID_DIV: out << "div"; goto print5;

            case MIID_SYSCALL: out << "syscall" << std::endl; return;
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
        out << std::endl;
        return;
    print1:
        // sll  sra  srl
        out << " ";
        _rd->print(out);
        out << ", ";
        _rt->print(out);
        out << ", ";
        _sa->print(out);
        out << std::endl;
        return;
    print2:
        // mfhi  mflo
        out << " ";
        _rd->print(out);
        out << std::endl;
        return;
    print3:
        // mthi  mtlo  jr
        out << " ";
        _rs->print(out);
        out << std::endl;
        return;
    print4:
        out << " ";
        _rt->print(out);
        out << ", ";
        _rd->print(out);
        out << std::endl;
        return;
    print5:
        out << " ";
        _rs->print(out);
        out << ", ";
        _rt->print(out);
        out << std::endl;
    }

    /******************** I-INST ********************/
    IInst::IInst(const MipsRegPtr& rs,
                 const MipsRegPtr& rt,
                 const MipsImmPtr& imm,
                 MipsInstID instID)
        : MipsInst(instID), _rs(rs), _rt(rt), _imm(imm) { }

    void IInst::print(std::ostream &out) {
        if (_label != nullptr) {
            _label->print(out);
        }
        out << "  ";
        switch (_id) {
            case MIID_ADDI: out << "andi "; goto print0;
            case MIID_SUBI: out << "subi "; goto print0;
            case MIID_ANDI: out << "andi "; goto print0;
            case MIID_ORI: out << "ori "; goto print0;
            case MIID_LW: out << "lw "; goto print1;
            case MIID_LB: out << "lb "; goto print1;
            case MIID_SW: out << "sw "; goto print1;
            case MIID_SB: out << "sb "; goto print1;

            case MIID_BEQ: out << "beq "; goto print2;
            case MIID_BNE: out << "bne "; goto print2;
            case MIID_BGE: out << "bge "; goto print2;
            case MIID_BGT: out << "bgt "; goto print2;
            case MIID_BLE: out << "ble "; goto print2;
            case MIID_BLT: out << "blt "; goto print2;
            case MIID_B: out << "b "; goto print3;

            case MIID_LI: out << "li "; goto print4;
            case MIID_LA: out << "la "; goto print4;
            default: assert(0);
        }
    print0:
        _rt->print(out);
        out << ", ";
        _rs->print(out);
        out << ", ";
        _imm->print(out);
        out << std::endl;
        return;
    print1:
        _rt->print(out);
        out << ", ";
        _imm->print(out);
        out << "(";
        _rs->print(out);
        out << ")";
        out << std::endl;
        return;
    print2:
        _rs->print(out);
        out << ", ";
        _rt->print(out);
        out << ", ";
        _imm->print(out);
        out << std::endl;
        return;
    print3:
        _imm->print(out);
        out << std::endl;
        return;
    print4:
        _rt->print(out);
        out << ", ";
        _imm->print(out);
        out << std::endl;
        return;
    }

    IInstPtr IInst::New(const MipsRegPtr &rs,
                        const MipsRegPtr &rt,
                        const MipsImmPtr &imm,
                        MipsInstID instID) {
        return std::make_shared<IInst>(rs, rt, imm, instID);
    }

    IInstPtr IInst::NewLi(const MipsRegPtr &rt, const MipsImmPtr &imm) {
        return New(REG_ZERO, rt, imm, MIID_LI);
    }

    IInstPtr IInst::NewLa(const MipsRegPtr &rt, const MipsImmPtr &imm) {
        return New(REG_ZERO, rt, imm, MIID_LA);
    }

    /******************** J-INST ********************/
    JInst::JInst(const MipsImmPtr imm, MipsInstID instID)
        : MipsInst(instID), _imm(imm) { }

    void JInst::print(std::ostream &out) {
        if (_label != nullptr) {
            _label->print(out);
        }
        out << "  ";
        switch (_id) {
            case MIID_J: out << "j "; goto print0;
            case MIID_JAL: out << "jal "; goto print0;
            default: assert(0);
        }
    print0:
        _imm->print(out);
    }

    JInstPtr JInst::New(const MipsImmPtr& imm, MipsInstID instID) {
        return std::make_shared<JInst>(imm, instID);
    }
}
