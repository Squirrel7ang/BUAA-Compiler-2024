//
// Created by tang on 11/26/24.
//

#ifndef MIPSASM_HPP
#define MIPSASM_HPP

#include "MipsCommon.hpp"

namespace mips {
    class MipsAsm {
        // TODO: global variable definition
        std::vector<MipsInstPtr> _insts;
    public:
        void addInst(MipsInstPtr);
    };
}

#endif //MIPSASM_HPP
