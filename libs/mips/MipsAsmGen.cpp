//
// Created by tang on 11/26/24.
//

#include "MipsAsmGen.hpp"

#include "IR/Instructions.hpp"
#include "IR/Module.hpp"

namespace mips {
    void MipsAsm::generateMipsAsm() {

    }

    void MipsAsm::print(std::ostream &out) {
    }

    void MipsAsm::translate() {
        _translator.translate();
    }

    void MipsAsm::allocateRegister() {
    }
}
