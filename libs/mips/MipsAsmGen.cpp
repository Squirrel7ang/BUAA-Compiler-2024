//
// Created by tang on 11/26/24.
//

#include "MipsAsmGen.hpp"

#include "IR/Instructions.hpp"
#include "IR/Module.hpp"

namespace mips {
    MipsAsm::MipsAsm(llvm::ModulePtr& module)
        : _llvmModule(module), _translator(MipsTranslator::New(module)) {

    }

    void MipsAsm::generateMipsAsm() {

    }

    void MipsAsm::print(std::ostream &out) {
    }

    void MipsAsm::translate() {
        _translator->translate();
    }

    void MipsAsm::allocateRegister() {
    }
}
