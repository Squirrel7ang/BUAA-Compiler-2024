//
// Created by tang on 11/26/24.
//

#ifndef MIPSINSTRUCTION_HPP
#define MIPSINSTRUCTION_HPP
#include <cassert>

#include "MipsCommon.hpp"

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
        virtual void print(std::ostream& out);
    };

    /**
     * rd = rs * rt
     * rd = rt * sa
     * jalr rd, rs ==> jump to rs, store PC in rd
     * jr rs
     * mfhi rd
     * mthi rs
     * mult rs, rt
     */
    class RInst : public MipsInst {
        MipsRegPtr _rs;
        MipsRegPtr _rt;
        MipsRegPtr _rd;
        MipsImmPtr _sa;

    public:
        static RInstPtr New(
            const MipsRegPtr& rs,
            const MipsRegPtr& rt,
            const MipsRegPtr& rd,
            MipsInstID instID
        );

        explicit RInst(
            const MipsRegPtr& rs,
            const MipsRegPtr& rt,
            const MipsRegPtr& rd,
            MipsInstID instID
        );
        void print(std::ostream& out) override;
    };

    /**
     * rt = rs * imm;
     * sw/lw rt, imm(rs);
     * beq rs, rt, offset;
     * lui rt, imm <==> lui rt, imm($zero)
     */
    class IInst : public MipsInst {
    private:
        MipsRegPtr _rs;
        MipsRegPtr _rt;
        MipsImmPtr _imm;
    public:
        static IInstPtr New(
            const MipsRegPtr& rs,
            const MipsRegPtr& rt,
            const MipsImmPtr& imm,
            MipsInstID instID
        );

        explicit IInst(
            const MipsRegPtr& rs,
            const MipsRegPtr& rt,
            const MipsImmPtr& imm,
            MipsInstID instID
        );
        void print(std::ostream& out) override;
    };

    /**
     * jal imm
     */
    class JInst : public MipsInst {
    private:
        MipsImmPtr _imm;
    public:
        static JInstPtr New(const MipsImmPtr& imm, MipsInstID instID);
        explicit JInst(const MipsImmPtr imm, MipsInstID instID);
        void print(std::ostream& out) override;
    };
}

#endif // MIPSINSTRUCTION_HPP
