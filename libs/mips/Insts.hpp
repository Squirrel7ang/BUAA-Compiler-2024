//
// Created by tang on 11/26/24.
//

#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP
#include <cassert>

#include "MipsCommon.hpp"
#include "Imm.hpp"
#include "Regs.hpp"

namespace mips {
    enum MipsInstID {
        // R-Inst
        MIID_ADD,
        MIID_SUB,
        MIID_MULT,
        MIID_DIV,

        MIID_AND,
        MIID_OR,
        MIID_XOR,

        MIID_SLL,
        MIID_SRA,
        MIID_SRL,

        MIID_MFHI,
        MIID_MFLO,
        MIID_MTHI,
        MIID_MTLO,

        MIID_JR,

        // I-Inst
        MIID_ADDI,
        MIID_SUBI,
        MIID_MULI,
        MIID_DIVI,

        MIID_ANDI,
        MIID_ORI,

        MIID_B,
        MIID_BEQ,
        MIID_BNE,
        MIID_BGE,
        MIID_BGT,
        MIID_BLE,
        MIID_BLT,

        // J-Inst
        MIID_J,
        MIID_JAL,
        MIID_JALR,
    };

    enum MipsInstType {
        MIT_R,
        MIT_I,
        MIT_J,
    };

    class MipsInst {
    protected:
        MipsInstType _type;
        MipsInstID _id;
    public:
        explicit MipsInst(MipsInstID id) : _id(id), _type(idToType(id)) { }
        static MipsInstType idToType(MipsInstID id);
        virtual void print(std::ostream& out) = 0;
    };

    class RInst : public MipsInst {
        MipsRegPtr _rs;
        MipsRegPtr _rt;
        MipsRegPtr _rd;
        MipsDataPtr _sa;

    public:
        RInstPtr New(MipsRegPtr& rs, MipsRegPtr& rt, MipsRegPtr& rd, MipsInstID instID);

    private:
        explicit RInst(MipsRegPtr& rs, MipsRegPtr& rt, MipsRegPtr& rd, MipsInstID instID)
            : MipsInst(instID), _rs(rs), _rt(rt), _rd(rd), _sa(DATA_ZERO) {
            assert(MipsInst::idToType(instID) == MIT_R);
        }
        void print(std::ostream& out) override;
    };

    class IInst : public MipsInst {
    private:
        MipsRegPtr _rs;
        MipsRegPtr _rt;
        MipsDataPtr _imm;
    public:
        IInstPtr New(MipsRegPtr& rs, MipsRegPtr& rt, MipsDataPtr& imm, MipsInstID instID);

    private:
        explicit IInst(MipsRegPtr& rs, MipsRegPtr& rt, MipsDataPtr& imm, MipsInstID instID)
            : MipsInst(instID), _rs(rs), _rt(rt), _imm(imm) { }
    };

    class JInst : public MipsInst {
    private:
        MipsDataPtr _imm;
    public:
        JInstPtr New(MipsDataPtr& imm, MipsInstID instID);
    private:
        JInst(MipsDataPtr imm, MipsInstID instID)
            : MipsInst(instID), _imm(imm) { }
    };
}

#endif //INSTRUCTIONS_HPP
