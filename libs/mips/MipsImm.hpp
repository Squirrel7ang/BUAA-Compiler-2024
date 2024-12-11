//
// Created by tang on 11/26/24.
//

#ifndef MIPSIMM_HPP
#define MIPSIMM_HPP

#include "MipsCommon.hpp"

namespace mips {
    class MipsImm {
        int _val;
    public:
        virtual ~MipsImm() = default;
        static MipsImmPtr New(int value);
        explicit MipsImm(int value);
        virtual void print(std::ostream& out) const;
    };

    static const MipsImmPtr IMM_ZERO = MipsImm::New(0);
    static const MipsImmPtr IMM_TRUNC_BYTE = MipsImm::New(0x000000ff);
}

#endif //MIPSIMM_HPP
