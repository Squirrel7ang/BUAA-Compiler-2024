//
// Created by tang on 11/26/24.
//

#ifndef MIPSASM_HPP
#define MIPSASM_HPP

#include "IR/Common.hpp"
#include "MipsCommon.hpp"
#include "MipsTranslator.hpp"

namespace mips {
    /**
     * This is the top module of MIPS asm generator
     */
    class MipsAsm {
        // TODO: global variable definition
        // llvm
        llvm::ModulePtr _llvmModule;
        MipsTranslator _translator;
    public:
        void generateMipsAsm();
        void print(std::ostream& out);

    // translation
    private:
        void translate();
        // register designation
    public:
        void allocateRegister();
    };

}

#endif //MIPSASM_HPP
