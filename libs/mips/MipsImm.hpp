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
        static MipsDataPtr New(int value);
        void print(std::ostream& out) const;
    private:
        explicit MipsImm(int value);
    };

    static const MipsDataPtr IMM_ZERO = MipsImm::New(0);
}

#endif //MIPSIMM_HPP
