//
// Created by tang on 11/26/24.
//

#ifndef MIPSASMGEN_HPP
#define MIPSASMGEN_HPP

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
        MipsTranslatorPtr _translator;
    public:
        explicit MipsAsm(llvm::ModulePtr& module);
        void generateMipsAsm();
        void print(std::ostream& out);
        MipsTranslatorPtr getTranslator() { return _translator; }
    };

}

#endif // MIPSASMGEN_HPP
